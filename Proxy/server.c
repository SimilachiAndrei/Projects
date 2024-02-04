#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <inttypes.h>
#include <netinet/in.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <sys/stat.h>

#define config "config.xml"
#define database "db.xml"
#define usersDb "users.xml"
#define MAX_BUFFER_SIZE 4096
#define FTP_SERVER "127.0.0.1"
#define FTP_PORT 21
#define MAX_URL_LENGTH 100
#define MAX_FILE_TYPE_COUNT 10
#define MAX_DAY_RANGE_LENGTH 100
#define MAX_TIME_RANGE_LENGTH 20
#define MAX_CLIENT_DOMAIN_LENGTH 20
#define PORT 2027
extern int errno;

typedef struct thData
{
    int idThread; 
    int cl;       
    char *username;
    char *proxyUser;
    int connected;
    char *domain;
} thData;

struct connByUsername
{
    int n;
    char username[50];
    time_t lastConnectionTime;
} onlineUsers[50];

int nrOfOnlineUsers = 0;

struct ProxyConfiguration
{
    int maxFileSize;
    char toKeep[50];
    struct
    {
        char fileTypes[MAX_FILE_TYPE_COUNT][10];
        int fileTypeCount;
    } allowedFileTypes;

    struct
    {
        char url[MAX_URL_LENGTH];
        int forbiddenSitesCount;
        struct
        {
            char dayRange[MAX_DAY_RANGE_LENGTH];
            char timeRange[MAX_TIME_RANGE_LENGTH];
            char clientDomain[MAX_CLIENT_DOMAIN_LENGTH];
        } accessPolicy;
    } forbiddenSites[MAX_FILE_TYPE_COUNT];
} p;

struct ProxyUsers
{
    char username[50];
} ProxyUser[100];

int nrOfProxyUsers = 0;

int connToProxy(const char *username, const char *password, int client, int *admin, char *tdlUser)
{
    xmlInitParser();
    xmlDocPtr doc;
    xmlNodePtr rootNode;

    int ok = 0;

    doc = xmlReadFile(usersDb, NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return -1;
    }

    rootNode = xmlDocGetRootElement(doc);
    for (xmlNodePtr node = rootNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE && strcmp(username, (const char *)node->name) == 0)
        {
            printf("%s\n", (const char *)node->name);
            xmlNodePtr childNode = node->children;
            while (childNode != NULL)
            {
                if (childNode->type == XML_TEXT_NODE)
                {
                    const char *nodeValue = (const char *)childNode->content;
                    if (strcmp(password, nodeValue) == 0)
                    {
                        printf("%s\n", nodeValue);
                        ok = 1;
                        printf("%d\n", ok);
                        if (strstr((const char *)node->name, "-admin") != 0)
                            *admin = 1;
                        else
                            *admin = 0;
                        printf("Nr of Proxy users : %d\n", nrOfProxyUsers);
                        if (nrOfProxyUsers == 0)
                        {
                            strcpy(ProxyUser[nrOfProxyUsers++].username, username);
                            strcpy(tdlUser, username);
                            write(client, "User connected successfully", strlen("User connected successfully"));
                            xmlSaveFormatFileEnc(usersDb, doc, "UTF-8", 1);
                            xmlFreeDoc(doc);
                            xmlCleanupParser();
                            return 1;
                        }
                        else if (nrOfProxyUsers != 0)
                        {
                            for (int i = 0; i < nrOfProxyUsers; i++)
                            {
                                if (strcmp(ProxyUser[i].username, username) == 0)
                                {
                                    printf("Compare : %s and %s\n", ProxyUser[i].username, username);
                                    write(client, "A user is already connected", strlen("A user is already connected"));
                                    xmlSaveFormatFileEnc(usersDb, doc, "UTF-8", 1);
                                    xmlFreeDoc(doc);
                                    xmlCleanupParser();
                                    return 0;
                                }
                            }
                            strcpy(ProxyUser[nrOfProxyUsers++].username, username);
                            strcpy(tdlUser, username);
                            write(client, "User connected successfully", strlen("User connected successfully"));
                            xmlSaveFormatFileEnc(usersDb, doc, "UTF-8", 1);
                            xmlFreeDoc(doc);
                            xmlCleanupParser();
                            return 1;
                        }
                    }
                }
                childNode = childNode->next;
            }
        }
    }

    xmlSaveFormatFileEnc(usersDb, doc, "UTF-8", 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();

    if (ok == 0)
        write(client, "User does not exist or incorrect password", strlen("User does not exist or incorrect password"));
    return 0;
}

void sendCommand(int sock, char *command)
{
    char buffer[MAX_BUFFER_SIZE];
    printf("%s\n", command);
    sprintf(buffer, "%s\r\n", command);
    send(sock, buffer, strlen(buffer), 0);
}

void parseForProxy(xmlNodePtr node, struct ProxyConfiguration *p)
{
    for (xmlNodePtr cur_node = node->children; cur_node; cur_node = cur_node->next)
    {
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            printf("Node type: Element, name: %s\n", cur_node->name);
            if (cur_node->children && cur_node->children->type == XML_TEXT_NODE)
            {
                printf("Content: %s\n", cur_node->children->content);
            }
            if (strcmp(cur_node->name, "maxFileSize") == 0)
            {
                p->maxFileSize = atoi(cur_node->children->content);
            }
            else if (strcmp(cur_node->name, "toKeep") == 0)
            {
                strcpy(p->toKeep, cur_node->children->content);
            }
            else if (strcmp(cur_node->name, "fileType") == 0)
            {
                strcpy(p->allowedFileTypes.fileTypes[p->allowedFileTypes.fileTypeCount++], cur_node->children->content);
            }
            else if (strcmp(cur_node->name, "url") == 0)
            {
                strcpy(p->forbiddenSites[p->forbiddenSites->forbiddenSitesCount].url, cur_node->children->content);
            }
            else if (strcmp(cur_node->name, "dayRange") == 0)
            {
                strcpy(p->forbiddenSites[p->forbiddenSites->forbiddenSitesCount].accessPolicy.dayRange, cur_node->children->content);
            }
            else if (strcmp(cur_node->name, "timeRange") == 0)
            {
                strcpy(p->forbiddenSites[p->forbiddenSites->forbiddenSitesCount].accessPolicy.timeRange, cur_node->children->content);
            }
            else if (strcmp(cur_node->name, "clientDomain") == 0)
            {
                strcpy(p->forbiddenSites[p->forbiddenSites->forbiddenSitesCount++].accessPolicy.clientDomain, cur_node->children->content);
            }
            printf("\n");
            parseForProxy(cur_node, p);
        }
    }
}

void parse(struct ProxyConfiguration *p, const char *path)
{
    xmlDocPtr doc;
    xmlNodePtr rootNode;

    doc = xmlReadFile(path, NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return;
    }

    rootNode = xmlDocGetRootElement(doc);
    if (rootNode == NULL)
    {
        fprintf(stderr, "Empty document.\n");
        xmlFreeDoc(doc);
        return;
    }

    parseForProxy(rootNode, p);

    xmlFreeDoc(doc);
}

void initialiseConfig(struct ProxyConfiguration *p)
{
    p->allowedFileTypes.fileTypeCount = 0;
    p->forbiddenSites->forbiddenSitesCount = 0;
    parse(p, "config.xml");
    printf("%d\n", p->maxFileSize);
    for (int i = 0; i < p->allowedFileTypes.fileTypeCount; i++)
    {
        printf("%s\n", p->allowedFileTypes.fileTypes[i]);
    }
    for (int i = 0; i < p->forbiddenSites->forbiddenSitesCount; i++)
    {
        printf("%s\n", p->forbiddenSites[i].url);
        printf("%s\n", p->forbiddenSites[i].accessPolicy.dayRange);
        printf("%s\n\n", p->forbiddenSites[i].accessPolicy.clientDomain);
    }
}

void user(int ftp, const char *name, int client)
{
    char buffer[100];
    bzero(buffer, 100);
    strcpy(buffer, "USER ");
    strcat(buffer, name);
    sendCommand(ftp, buffer);
    bzero(buffer, 100);
    // Wait for the server response
    read(ftp, buffer, sizeof(buffer));
    printf("Server: %s", buffer);
    write(client, buffer, sizeof(buffer));
}

void pass(int ftp, const char *pass, int client, char *username)
{
    char buffer[100];
    bzero(buffer, 100);
    strcpy(buffer, "PASS ");
    strcat(buffer, pass);
    sendCommand(ftp, buffer);
    bzero(buffer, 100);
    read(ftp, buffer, sizeof(buffer));
    printf("Server: %s", buffer);
    if (strstr(buffer, "530") != 0)
        bzero(username, 50);
    /*else{
        put all the files in the database and then later modify retr to search in the database and
        list to list all the files in the database for the client
    }*/
    write(client, buffer, sizeof(buffer));
}

int initialiseConnection(int ftp)
{
    int data_sock;
    char buffer[MAX_BUFFER_SIZE];
    struct sockaddr_in dataAddr;
    bzero(&dataAddr, sizeof(struct sockaddr_in));
    sendCommand(ftp, "PASV");

    bzero(buffer, MAX_BUFFER_SIZE);
    read(ftp, buffer, sizeof(buffer));
    printf("Server: %s", buffer);

    unsigned int ip[4];
    int port1, port2;
    sscanf(buffer, "227 Entering Passive Mode (%u,%u,%u,%u,%u,%u)",
           &ip[0], &ip[1], &ip[2], &ip[3], &port1, &port2);

    int port = port1 * 256 + port2;

    if ((data_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Data socket creation error");
        return errno;
    }

    memset(&dataAddr, '0', sizeof(dataAddr));
    dataAddr.sin_family = AF_INET;
    dataAddr.sin_port = htons(port);
    dataAddr.sin_addr.s_addr = htonl((ip[0] << 24) | (ip[1] << 16) | (ip[2] << 8) | ip[3]);

    if (connect(data_sock, (struct sockaddr *)&dataAddr, sizeof(dataAddr)) < 0)
    {
        perror("Data connection failed");
        return errno;
    }
    return data_sock;
}

int verifyHostByProxy(char *host, char *domain)
{
    for (int i = 0; i < p.forbiddenSites->forbiddenSitesCount; i++)
    {
        if (strstr(host, p.forbiddenSites[i].url) != 0)
        {
            time_t currentTime = time(NULL);
            struct tm *localTime = localtime(&currentTime);
            char dayOfWeekString[100];
            strftime(dayOfWeekString, sizeof(dayOfWeekString), "%A", localTime);
            char timeInHoursString[3];
            strftime(timeInHoursString, sizeof(timeInHoursString), "%H", localTime);
            int startHour, endHour, currHour;
            sscanf(p.forbiddenSites[i].accessPolicy.timeRange, "%d-%d", &startHour, &endHour);
            sscanf(timeInHoursString, "%d", &currHour);
            printf("Day of week:%s\n", dayOfWeekString);
            if (strstr(p.forbiddenSites[i].accessPolicy.dayRange, dayOfWeekString) != 0 &&
                currHour >= startHour && currHour <= endHour)
            {
                if (strstr(domain, p.forbiddenSites[i].accessPolicy.clientDomain) != 0)
                {
                    return 0;
                }
            }
        }
    }
    return 1;
}

int initConnProxyFTP(int client, char *host, int *con, char *userDomain)
{
    int ftp;
    struct sockaddr_in ftpAddr, dataAddr;
    char buff[MAX_BUFFER_SIZE];

    if ((ftp = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        return errno;
    }

    bzero(&ftpAddr, sizeof(ftpAddr));
    if (strcmp(host, "127.0.0.1") != 0 && strcmp(host, "0") != 0)
    {
        if (verifyHostByProxy(host, userDomain) == 1)
        {
            struct hostent *serverHost;
            host[strlen(host) - 1] = '\0';
            printf("%s\n", host);
            serverHost = gethostbyname(host);
            if (serverHost == NULL)
            {
                perror("Error resolving host name");
                write(client, "Error resolving host name!", strlen("Error resolving host name!"));
                return errno;
            }
            else
                *con = 1;
            ftpAddr.sin_family = AF_INET;
            ftpAddr.sin_port = htons(FTP_PORT);
            memcpy(&ftpAddr.sin_addr, serverHost->h_addr_list[0], serverHost->h_length);
        }
        else
        {
            write(client, "Acces denied due to proxy rules\n", strlen("Acces denied due to proxy rules\n"));
            return errno;
        }
    }
    else
    {
        ftpAddr.sin_family = AF_INET;
        ftpAddr.sin_port = htons(FTP_PORT);
        ftpAddr.sin_addr.s_addr = inet_addr(FTP_SERVER);
    }
    if (connect(ftp, (struct sockaddr *)&ftpAddr, sizeof(ftpAddr)) < 0)
    {
        perror("Connection failed");
        *con = 0;
        write(client, "Connection failed", strlen("Connection failed"));
        return errno;
    }

    bzero(buff, MAX_BUFFER_SIZE);

    read(ftp, buff, sizeof(buff));

    printf("Server: %s", buff);
    write(client, buff, strlen(buff));
    return ftp;
}

int initServCltCommunication(struct sockaddr_in *server)
{
    int sd;
    if ((sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("[server]Eroare la socket().\n");
        return errno;
    }
    int op = 1;
    setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(op));
    bzero(server, sizeof(server));
    server->sin_family = AF_INET;
    server->sin_addr.s_addr = htonl(INADDR_ANY);
    server->sin_port = htons(PORT);

    if (bind(sd, (struct sockaddr *)server, sizeof(struct sockaddr)) == -1)
    {
        perror("[server]Eroare la bind().\n");
        return errno;
    }

    if (listen(sd, 5) == -1)
    {
        perror("[server]Eroare la listen().\n");
        return errno;
    }
    return sd;
}

void initialise_database()
{
    xmlInitParser();
    xmlDocPtr doc = xmlNewDoc(BAD_CAST "1.0");
    xmlNodePtr root = xmlNewNode(NULL, BAD_CAST "db");
    xmlDocSetRootElement(doc, root);
    xmlSaveFormatFile(database, doc, 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

int verify_if_user_exists(const char *user)
{
    xmlInitParser();
    xmlDocPtr doc;
    xmlNodePtr rootNode;
    doc = xmlReadFile(database, NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return -1;
    }
    rootNode = xmlDocGetRootElement(doc);
    for (xmlNodePtr node = rootNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE && strcmp(user, node->name) == 0)
        {
            xmlSaveFormatFile(database, doc, 1);
            xmlFreeDoc(doc);
            xmlCleanupParser();
            return 1;
        }
    }
    xmlSaveFormatFile(database, doc, 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

void add_user(const char *username)
{
    if (verify_if_user_exists(username) == 0)
    {
        xmlInitParser();
        xmlDocPtr doc;
        xmlNodePtr rootNode;
        doc = xmlReadFile(database, NULL, 0);
        if (doc == NULL)
        {
            fprintf(stderr, "Failed to parse the XML file.\n");
            return;
        }
        rootNode = xmlDocGetRootElement(doc);
        xmlNodePtr user = xmlNewNode(NULL, BAD_CAST username);
        xmlAddChild(rootNode, user);
        xmlSaveFormatFile(database, doc, 1);
        xmlFreeDoc(doc);
        xmlCleanupParser();
    }
}

void add_file_to_user(const char *filename, char *content, const char *user)
{
    xmlInitParser();
    xmlDocPtr doc;
    xmlNodePtr rootNode;
    doc = xmlReadFile(database, NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return;
    }
    rootNode = xmlDocGetRootElement(doc);
    for (xmlNodePtr node = rootNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE && strcmp(user, node->name) == 0)
        {
            xmlNodePtr newNode = xmlNewNode(NULL, BAD_CAST filename);
            xmlAddChild(node, newNode);
            xmlNodePtr newContent = xmlNewText(BAD_CAST content);
            xmlAddChild(newNode, newContent);
        }
    }
    xmlSaveFormatFile(database, doc, 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

void putInDb(int ftp, const char *file, const char *path, const char *user)
{
    int data_sock = initialiseConnection(ftp);
    char buffer[MAX_BUFFER_SIZE];
    bzero(buffer, MAX_BUFFER_SIZE);
    strcpy(buffer, "RETR ");
    strcat(buffer, path);
    sendCommand(ftp, buffer);

    bzero(buffer, MAX_BUFFER_SIZE);
    read(ftp, buffer, sizeof(buffer));
    printf("Server: %s", buffer);

    if (buffer[0] == '1' && buffer[1] == '5' && buffer[2] == '0')
    {
        bzero(buffer, MAX_BUFFER_SIZE);
        ssize_t bytesRead;
        while ((bytesRead = read(data_sock, buffer, sizeof(buffer))) > 0)
        {
            add_user(user);
            add_file_to_user(file, buffer, user);
            fwrite(buffer, 1, bytesRead, stdout);
            bzero(buffer, MAX_BUFFER_SIZE);
        }
    }
    else
    {
        printf("Error: Failed to retrieve file\n");
    }
    bzero(buffer, MAX_BUFFER_SIZE);
    read(ftp, buffer, sizeof(buffer));
    printf("Server: %s", buffer);
    close(data_sock);
}

void searchFiles(int ftp, const char *username, const char *property, const char *dir)
{
    int data_sock = initialiseConnection(ftp);
    char buffer[MAX_BUFFER_SIZE];
    char command[50];
    bzero(command, 50);
    strcpy(command, "LIST ");
    strcat(command, dir);
    char subdires[50][50];
    char path[50][50];
    char files[50][50];
    bzero(subdires, 50 * 50);
    bzero(path, 50 * 50);
    bzero(files, 50 * 50);
    int contor = 0, f_contor = 0;
    sendCommand(ftp, command);
    bzero(buffer, MAX_BUFFER_SIZE);

    read(ftp, buffer, sizeof(buffer));
    printf("Server: %s", buffer);

    if (buffer[0] == '1' && buffer[1] == '5' && buffer[2] == '0')
    {
        bzero(buffer, MAX_BUFFER_SIZE);
        ssize_t bytesRead;
        while ((bytesRead = read(data_sock, buffer, sizeof(buffer))) > 0)
        {
            char *token = strtok(buffer, "\n");
            while (token != NULL)
            {
                printf("File or directory: %s\n", token);

                if (strchr(token, '.') == 0)
                {
                    printf("Adding subdirectory: %s\n", token);
                    char directory[50];
                    bzero(directory, 50);
                    int ct = 0;
                    for (int i = strlen(token) - 1; strncmp(token + i, " ", 1); i--)
                        ct++;
                    char dire[100];
                    bzero(dire, 100);
                    if (dire[strlen(dire) - 1] != '/' && strcmp(dir, "\0") != 0)
                        snprintf(dire, strlen(dir) + 1 + strlen(token + strlen(token) - ct), "%s/%s", dir, token + strlen(token) - ct);
                    else
                    {
                        snprintf(dire, strlen(token + strlen(token) - ct), "%s", token + strlen(token) - ct);
                    }
                    printf("Subdir : %s\n", token + strlen(token) - ct);

                    printf("Path : %s\n", dire);

                    strcpy(subdires[contor++], dire);
                }
                else
                {
                    printf("Found a file with property '%s': %s\n", property, token);
                    int ct = 0;
                    for (int i = strlen(token) - 1; strncmp(token + i, " ", 1); i--)
                        ct++;
                    int ok1 = 0, ok2 = 0;
                    for (int i = 0; i < p.allowedFileTypes.fileTypeCount; i++)
                    {
                        if (strstr(token + strlen(token) - ct, p.allowedFileTypes.fileTypes[i]) != 0)
                            ok1 = 1;
                    }
                    int size;
                    sscanf(token, "%*s %*s %*s %*s %d", &size);
                    printf("Size of the file: %d bytes\n", size);

                    if (ok1 == 1 && size <= p.maxFileSize)
                    {
                        strcpy(files[f_contor], token + strlen(token) - ct);
                        if (dir[strlen(dir) - 1] != '/' && strcmp(dir, "\0") != 0)
                            snprintf(path[f_contor++], strlen(dir) + 1 + strlen(token + strlen(token) - ct), "%s/%s", dir, token + strlen(token) - ct);
                        else
                        {
                            snprintf(path[f_contor++], strlen(token + strlen(token) - ct), "%s", token + strlen(token) - ct);
                        }
                    }
                }

                token = strtok(NULL, "\n");
            }
            bzero(buffer, MAX_BUFFER_SIZE);
        }
    }
    else
    {
        printf("Error: Failed to retrieve file\n");
    }
    bzero(buffer, MAX_BUFFER_SIZE);
    read(ftp, buffer, sizeof(buffer));
    printf("Server: %s", buffer);
    close(data_sock);
    for (int i = 0; i < f_contor; i++)
    {
        putInDb(ftp, files[i], path[i], username);
    }
    for (int i = 0; i < contor; i++)
    {
        searchFiles(ftp, username, property, subdires[i]);
    }
}

void retr(int client, const char *filename, const char *username)
{
    xmlInitParser();
    xmlDocPtr doc;
    xmlNodePtr rootNode;
    doc = xmlReadFile(database, NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return;
    }
    int found = 0;
    rootNode = xmlDocGetRootElement(doc);
    for (xmlNodePtr node = rootNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE && strcmp(username, node->name) == 0)
        {
            for (xmlNodePtr file = node->children; file; file = file->next)
            {
                if (file->type == XML_ELEMENT_NODE)
                {
                    const char *currentFilename = (char *)file->name;
                    const char *currentContent = (char *)xmlNodeGetContent(file->children);

                    if (strcmp(filename, currentFilename) == 0)
                    {
                        printf("%s\n%s\n", currentFilename, currentContent);
                        found = 1;
                        write(client, currentContent, strlen(currentContent));
                    }
                }
            }
        }
    }
    if (found == 0)
        write(client, "There is no such file", strlen("There is no such file"));
    xmlSaveFormatFile(database, doc, 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

void list(int client, const char *username)
{
    xmlInitParser();
    xmlDocPtr doc;
    xmlNodePtr rootNode;
    doc = xmlReadFile(database, NULL, 0);
    char buffer[1000];
    bzero(buffer, 1000);
    int k = 1;
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return;
    }
    rootNode = xmlDocGetRootElement(doc);
    for (xmlNodePtr node = rootNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE && strcmp(username, node->name) == 0)
        {
            for (xmlNodePtr file = node->children; file; file = file->next)
            {
                if (file->type == XML_ELEMENT_NODE)
                {
                    const char *currentFilename = (char *)file->name;
                    printf("%s\n", currentFilename);
                    if (k == 1)
                    {
                        strcpy(buffer, "\n");
                        strcat(buffer, currentFilename);
                        k++;
                    }
                    else
                    {
                        strcat(buffer, "\n");
                        strcat(buffer, currentFilename);
                    }
                }
            }
        }
    }
    write(client, buffer, strlen(buffer));
    xmlSaveFormatFile(database, doc, 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

void removeFromDb(const char *user)
{
    xmlInitParser();
    xmlDocPtr doc;
    xmlNodePtr rootNode;
    doc = xmlReadFile(database, NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return;
    }
    rootNode = xmlDocGetRootElement(doc);
    for (xmlNodePtr node = rootNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE && strcmp(user, node->name) == 0)
        {
            xmlUnlinkNode(node);
            xmlFreeNode(node);
        }
    }
    xmlSaveFormatFile(database, doc, 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();
}

void modifyMaxFileSize(const char *modifyMaxFileSize, struct ProxyConfiguration *p)
{
    xmlInitParser();
    xmlDocPtr doc;
    xmlNodePtr rootNode;
    doc = xmlReadFile(config, NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return;
    }
    rootNode = xmlDocGetRootElement(doc);
    for (xmlNodePtr node = rootNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE && strcmp("maxFileSize", (const char *)node->name) == 0)
        {
            xmlNodeSetContent(node, NULL);
            xmlNodeSetContent(node, BAD_CAST modifyMaxFileSize);
        }
    }
    xmlSaveFormatFileEnc(config, doc, "UTF-8", 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    initialiseConfig(p);
}

void modifyToKeep(const char *modToKeep, struct ProxyConfiguration *p)
{
    xmlInitParser();
    xmlDocPtr doc;
    xmlNodePtr rootNode;
    doc = xmlReadFile(config, NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return;
    }
    rootNode = xmlDocGetRootElement(doc);
    for (xmlNodePtr node = rootNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE && strcmp("toKeep", (const char *)node->name) == 0)
        {
            xmlNodeSetContent(node, NULL);
            xmlNodeSetContent(node, BAD_CAST modToKeep);
        }
    }
    xmlSaveFormatFileEnc(config, doc, "UTF-8", 1);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    initialiseConfig(p);
}

int verify_if_fileType_exists(const char *fileType)
{
    xmlInitParser();
    xmlDocPtr doc;
    xmlNodePtr rootNode;
    doc = xmlReadFile(config, NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return -1;
    }
    rootNode = xmlDocGetRootElement(doc);
    for (xmlNodePtr node = rootNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE && strcmp("allowedFileTypes", (const char *)node->name) == 0)
        {
            for (xmlNodePtr fileTypeNode = node->children; fileTypeNode; fileTypeNode = fileTypeNode->next)
            {
                if (fileTypeNode->type == XML_ELEMENT_NODE && strcmp("fileType", (const char *)fileTypeNode->name) == 0)
                {
                    xmlNodePtr contentNode = fileTypeNode->children;
                    if (contentNode && contentNode->type == XML_TEXT_NODE)
                    {
                        const char *existingFileType = (const char *)contentNode->content;
                        if (strcmp(existingFileType, fileType) == 0)
                        {
                            xmlFreeDoc(doc);
                            xmlCleanupParser();
                            return 1;
                        }
                    }
                }
            }
        }
    }
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

void addFileType(const char *fileType, struct ProxyConfiguration *p, int client)
{
    if (verify_if_fileType_exists(fileType) == 0)
    {
        xmlInitParser();
        xmlDocPtr doc;
        xmlNodePtr rootNode;
        doc = xmlReadFile(config, NULL, 0);
        if (doc == NULL)
        {
            fprintf(stderr, "Failed to parse the XML file.\n");
            return;
        }
        rootNode = xmlDocGetRootElement(doc);
        for (xmlNodePtr node = rootNode->children; node; node = node->next)
        {
            if (node->type == XML_ELEMENT_NODE && strcmp("allowedFileTypes", (const char *)node->name) == 0)
            {
                xmlNodePtr newNode = xmlNewNode(NULL, BAD_CAST "fileType");
                xmlAddChild(node, newNode);
                xmlNodePtr newContent = xmlNewText(BAD_CAST fileType);
                xmlAddChild(newNode, newContent);
                write(client, "File type added", strlen("File type added"));
            }
        }
        xmlSaveFormatFileEnc(config, doc, "UTF-8", 1);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        initialiseConfig(p);
    }
    else
        write(client, "File type adready exists", strlen("File type adready exists"));
};

int deleteFileType(const char *fileType, struct ProxyConfiguration *p, int client)
{
    xmlInitParser();
    xmlDocPtr doc;
    xmlNodePtr rootNode;
    doc = xmlReadFile(config, NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return -1;
    }
    rootNode = xmlDocGetRootElement(doc);
    for (xmlNodePtr node = rootNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE && strcmp("allowedFileTypes", (const char *)node->name) == 0)
        {
            for (xmlNodePtr fileTypeNode = node->children; fileTypeNode; fileTypeNode = fileTypeNode->next)
            {
                if (fileTypeNode->type == XML_ELEMENT_NODE && strcmp("fileType", (const char *)fileTypeNode->name) == 0)
                {
                    xmlNodePtr contentNode = fileTypeNode->children;
                    if (contentNode && contentNode->type == XML_TEXT_NODE)
                    {
                        const char *existingFileType = (const char *)contentNode->content;
                        if (strcmp(existingFileType, fileType) == 0)
                        {
                            xmlUnlinkNode(fileTypeNode);
                            xmlFreeNode(fileTypeNode);

                            xmlSaveFormatFileEnc(config, doc, "UTF-8", 1);

                            xmlFreeDoc(doc);
                            xmlCleanupParser();
                            initialiseConfig(p);
                            write(client, "File type deleted", strlen("File type deleted"));
                            return 1;
                        }
                    }
                }
            }
        }
    }
    xmlSaveFormatFileEnc(config, doc, "UTF-8", 1);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    write(client, "File type does not exist", strlen("File type does not exist"));
    return 0;
}

int verifyIfUrlExists(const char *url)
{
    xmlInitParser();
    xmlDocPtr doc;
    xmlNodePtr rootNode;
    doc = xmlReadFile(config, NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return -1;
    }
    rootNode = xmlDocGetRootElement(doc);
    for (xmlNodePtr node = rootNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE && strcmp("forbiddenSites", (const char *)node->name) == 0)
        {
            for (xmlNodePtr fbSite = node->children; fbSite; fbSite = fbSite->next)
            {
                if (fbSite->type == XML_ELEMENT_NODE && strcmp("site", (const char *)fbSite->name) == 0)
                {
                    for (xmlNodePtr fbUrl = fbSite->children; fbUrl; fbUrl = fbUrl->next)
                    {
                        if (fbUrl->type == XML_ELEMENT_NODE && strcmp("url", (const char *)fbUrl->name) == 0)
                        {
                            xmlNodePtr contentNode = fbUrl->children;
                            if (contentNode && contentNode->type == XML_TEXT_NODE)
                            {
                                const char *furl = (const char *)contentNode->content;
                                if (strcmp(furl, url) == 0)
                                {
                                    xmlFreeDoc(doc);
                                    xmlCleanupParser();
                                    return 1;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

void addForbiddenUrl(const char *url, const char *dayRange, const char *timeRange,
                     const char *clientDomain, struct ProxyConfiguration *p, int client)
{
    if (verifyIfUrlExists(url) == 0)
    {
        xmlInitParser();
        xmlDocPtr doc;
        xmlNodePtr rootNode;
        doc = xmlReadFile(config, NULL, 0);
        if (doc == NULL)
        {
            fprintf(stderr, "Failed to parse the XML file.\n");
            return;
        }
        rootNode = xmlDocGetRootElement(doc);
        for (xmlNodePtr node = rootNode->children; node; node = node->next)
        {
            if (node->type == XML_ELEMENT_NODE && strcmp("forbiddenSites", (const char *)node->name) == 0)
            {
                for (xmlNodePtr site = node->children; site; site = site->next)
                {
                    if (site->type == XML_ELEMENT_NODE && strcmp("site", (const char *)site->name) == 0)
                    {
                        xmlNodePtr newUrlNode = xmlNewNode(NULL, BAD_CAST "url");
                        xmlNodePtr newPolicyNode = xmlNewNode(NULL, BAD_CAST "accessPolicy");

                        xmlAddChild(site, newUrlNode);
                        xmlAddChild(site, newPolicyNode);

                        xmlNodePtr urlContent = xmlNewText(BAD_CAST url);
                        xmlNodePtr newDRange = xmlNewNode(NULL, BAD_CAST "dayRange");
                        xmlNodePtr newTrange = xmlNewNode(NULL, BAD_CAST "timeRange");
                        xmlNodePtr newCDomain = xmlNewNode(NULL, BAD_CAST "clientDomain");

                        xmlAddChild(newUrlNode, urlContent);
                        xmlAddChild(newPolicyNode, newDRange);
                        xmlAddChild(newPolicyNode, newTrange);
                        xmlAddChild(newPolicyNode, newCDomain);

                        xmlNodePtr DContent = xmlNewText(BAD_CAST dayRange);
                        xmlNodePtr TContent = xmlNewText(BAD_CAST timeRange);
                        xmlNodePtr CContent = xmlNewText(BAD_CAST clientDomain);

                        xmlAddChild(newDRange, DContent);
                        xmlAddChild(newTrange, TContent);
                        xmlAddChild(newCDomain, CContent);
                        write(client, "Forbidden site added", strlen("Forbidden site added"));
                    }
                }
            }
        }
        xmlSaveFormatFileEnc(config, doc, "UTF-8", 1);
        xmlFreeDoc(doc);
        xmlCleanupParser();
        initialiseConfig(p);
    }
    else
        write(client, "Forbidden site already exists", strlen("Forbidden site already exists"));
}

int deleteForbiddenUrl(const char *url, struct ProxyConfiguration *p, int client)
{
    xmlInitParser();
    xmlDocPtr doc;
    xmlNodePtr rootNode;
    doc = xmlReadFile(config, NULL, 0);
    if (doc == NULL)
    {
        fprintf(stderr, "Failed to parse the XML file.\n");
        return -1;
    }
    rootNode = xmlDocGetRootElement(doc);
    for (xmlNodePtr node = rootNode->children; node; node = node->next)
    {
        if (node->type == XML_ELEMENT_NODE && strcmp("forbiddenSites", (const char *)node->name) == 0)
        {
            for (xmlNodePtr site = node->children; site; site = site->next)
            {
                if (site->type == XML_ELEMENT_NODE && strcmp("site", (const char *)site->name) == 0)
                {
                    xmlNodePtr urlNode = NULL;
                    xmlNodePtr policyNode = NULL;

                    for (xmlNodePtr child = site->children; child; child = child->next)
                    {
                        if (child->type == XML_ELEMENT_NODE && strcmp("url", (const char *)child->name) == 0)
                        {
                            xmlNodePtr contentNode = child->children;
                            if (contentNode && contentNode->type == XML_TEXT_NODE)
                            {
                                const char *existingUrl = (const char *)contentNode->content;
                                if (strcmp(existingUrl, url) == 0)
                                {
                                    urlNode = child;
                                    break;
                                }
                            }
                        }
                    }

                    if (urlNode != NULL)
                    {
                        for (xmlNodePtr child = urlNode->next; child; child = child->next)
                        {
                            if (child->type == XML_ELEMENT_NODE && strcmp("accessPolicy", (const char *)child->name) == 0)
                            {
                                policyNode = child;
                                xmlUnlinkNode(urlNode);
                                xmlFreeNode(urlNode);
                                break;
                            }
                        }

                        if (policyNode != NULL)
                        {
                            xmlUnlinkNode(policyNode);
                            xmlFreeNode(policyNode);

                            xmlSaveFormatFileEnc(config, doc, "UTF-8", 1);

                            xmlFreeDoc(doc);
                            xmlCleanupParser();
                            initialiseConfig(p);
                        }
                        write(client, "Url deleted", strlen("Url deleted"));
                        return 1;
                    }
                }
            }
        }
    }

    xmlSaveFormatFileEnc(config, doc, "UTF-8", 1);

    xmlFreeDoc(doc);
    xmlCleanupParser();
    write(client, "Url does not exist", strlen("Url does not exist"));
}

static void *treat(void *); 

int handleClient(void *);

void *continuousExecution(void *arg);

int main()
{
    /*pthread_t tid;
    if (pthread_create(&tid, NULL, continuousExecution, NULL) != 0)
    {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }*/
    pthread_t th[100]; // Identificatorii thread-urilor care se vor crea
    int i = 0;
    initialiseConfig(&p);
    struct sockaddr_in server; // structura folosita de server
    struct sockaddr_in from;
    bzero(&from, sizeof(from));
    int sd = initServCltCommunication(&server);
    initialise_database();
    /* servim in mod concurent clientii... */
    while (1)
    {
        int client;
        int length = sizeof(from);
        thData *td; // parametru functia executata de thread
        printf("[server]Asteptam la portul %d...\n", PORT);
        fflush(stdout);

        /* acceptam un client (stare blocanta pina la realizarea conexiunii) */
        client = accept(sd, (struct sockaddr *)&from, &length);

        /* eroare la acceptarea conexiunii de la un client */
        if (client < 0)
        {
            perror("[server]Eroare la accept().\n");
            return errno;
        }
        td = (struct thData *)malloc(sizeof(struct thData));
        td->idThread = i;
        td->cl = client;
        td->connected = 0;
        td->username = (char *)malloc(50 * sizeof(char));
        td->proxyUser = (char *)malloc(50 * sizeof(char));
        bzero(td->username, 50);
        td->domain = (char *)malloc(50 * sizeof(char));
        bzero(td->domain, 50);

        char h[100];
        bzero(h, 100);
        socklen_t addr_len = sizeof(struct sockaddr_in);
        if (getnameinfo((struct sockaddr *)&from, addr_len, h, sizeof(h), NULL, 0, 0) == 0)
        {
            strcpy(td->domain, h);
            printf("Client domain: %s\n", td->domain);
        }
        else
        {
            perror("getnameinfo\n");
        }
        pthread_create(&th[i], NULL, &treat, td);
        i++;
    } /* while */
    if (remove(database) == 0)
    {
        printf("XML database deleted successfully.\n");
    }
    else
    {
        perror("Error deleting XML database");
        return EXIT_FAILURE;
    }
    /*if (pthread_join(tid, NULL) != 0)
    {
        fprintf(stderr, "Error joining thread\n");
        return 1;
    }*/
}

static void *treat(void *arg)
{
    struct thData tdL;
    tdL = *((struct thData *)arg);
    printf("[thread]- %d - Asteptam mesajul...\n", tdL.idThread);
    fflush(stdout);
    pthread_detach(pthread_self());
    handleClient((struct thData *)arg);
    close((intptr_t)arg);
    return (NULL);
};

void *continuousExecution(void *arg)
{
    while (1)
    {
        for (int i = 0; i < nrOfOnlineUsers; i++)
        {
            if (onlineUsers[i].n == 0)
            {
                time_t currentTime = time(NULL);
                onlineUsers[i].lastConnectionTime = currentTime;

                int tokeepSeconds = atoi(p.toKeep);

                while (onlineUsers[i].n == 0)
                {
                    sleep(1);
                    currentTime = time(NULL);
                    int elapsedSeconds = currentTime - onlineUsers[i].lastConnectionTime;

                    if (elapsedSeconds >= tokeepSeconds && onlineUsers[i].n == 0)
                    {
                        printf("Time has passed. Performing necessary actions.\n");
                        removeFromDb(onlineUsers[i].username);
                        break;
                    }
                }
            }
        }
    }

    pthread_exit(NULL);
}

int handleClient(void *arg)
{
    struct thData tdL;
    tdL.connected = 0;
    tdL = *((struct thData *)arg);
    int client = tdL.cl;
    char msg[100];
    char msgrasp[100];
    char prUsr[50], passd[50];
    bzero(prUsr, sizeof(prUsr));
    bzero(passd, sizeof(passd));
    int ftp, step = 0;
    int admin;
    int proxyCon = 0;
    while (1)
    {
        bzero(msg, sizeof(msg));
        int nrOf;
        printf("[server]Asteptam mesajul...\n");
        fflush(stdout);

        if (read(client, msg, sizeof(msg)) < 0)
        {
            perror("[server]Eroare la read() de la client.\n");
            return errno;
        }
        printf("[server]Mesajul a fost receptionat...%s\n", msg);

        if (strncmp("modifyMaxFileSize", msg, strlen("modifyMaxFileSize")) == 0 && admin == 1 && proxyCon == 1)
        {
            msg[strlen(msg) - 1] = '\0';
            char maxfilesize[10];
            bzero(maxfilesize, sizeof(maxfilesize));
            sscanf(msg, "modifyMaxFileSize %s", maxfilesize);
            printf("Mfsize: %s\n", maxfilesize);
            modifyMaxFileSize(maxfilesize, &p);
            write(client, "Max file size modified", strlen("Max file size modified"));
        }
        else if (strncmp("modifyToKeep", msg, strlen("modifyToKeep")) == 0 && admin == 1 && proxyCon == 1)
        {
            msg[strlen(msg) - 1] = '\0';
            char mToKeep[10];
            bzero(mToKeep, sizeof(mToKeep));
            sscanf(msg, "modifyToKeep %s", mToKeep);
            printf("MtoKeep: %s\n", mToKeep);
            modifyToKeep(mToKeep, &p);
            write(client, "ToKeep modified", strlen("ToKeep modified"));
        }
        else if (strncmp("addFileType", msg, strlen("addFileType")) == 0 && admin == 1 && proxyCon == 1)
        {
            msg[strlen(msg) - 1] = '\0'; 
            char fileType[10];  
            bzero(fileType, sizeof(fileType));
            sscanf(msg, "addFileType %s", fileType);
            printf("FileType: %s\n", fileType);
            addFileType(fileType, &p, client);
        }
        else if (strncmp("deleteFileType", msg, strlen("deleteFileType")) == 0 && admin == 1 && proxyCon == 1)
        {
            msg[strlen(msg) - 1] = '\0';
            char fileType[10]; 
            bzero(fileType, sizeof(fileType));
            sscanf(msg, "deleteFileType %s", fileType); 
            printf("FileType: %s\n", fileType);
            deleteFileType(fileType, &p, client);
        }
        else if (strncmp("addForbiddenUrl", msg, strlen("addForbiddenUrl")) == 0 && admin == 1 && proxyCon == 1)
        {
            msg[strlen(msg) - 1] = '\0';
            char url[50];
            bzero(url, sizeof(url));
            char drange[50];
            bzero(drange, sizeof(drange));
            char trange[10]; 
            bzero(trange, sizeof(trange));
            char cdom[10];
            bzero(cdom, sizeof(cdom));
            sscanf(msg, "addForbiddenUrl %s %s %s %s", url, drange, trange, cdom);
            addForbiddenUrl(url, drange, trange, cdom, &p, client);
        }
        else if (strncmp("deleteForbiddenUrl", msg, strlen("deleteForbiddenUrl")) == 0 && admin == 1 && proxyCon == 1)
        {
            msg[strlen(msg) - 1] = '\0';
            char url[50];
            bzero(url, sizeof(url));
            sscanf(msg, "deleteForbiddenUrl %s", url);
            deleteForbiddenUrl(url, &p, client);
        }
        else if ((step == 0 || tdL.connected == 0) && strcmp(msg, "quit\n") != 0 && proxyCon == 1)
        {
            ftp = initConnProxyFTP(client, msg, &tdL.connected, tdL.domain);
            step++;
        }
        else if (strcmp(msg, "quit\n") == 0)
        {
            for (int i = 0; i < nrOfProxyUsers; i++)
            {
                if (strcmp(tdL.proxyUser, ProxyUser[i].username) == 0)
                {
                    printf("%s\n", tdL.proxyUser);
                    for (int j = i; j < nrOfProxyUsers; j++)
                    {
                        strcpy(ProxyUser[j].username, ProxyUser[j + 1].username);
                    }
                }
            }
            free((void *)tdL.proxyUser);
            for (int i = 0; i < nrOfOnlineUsers; i++)
            {
                if (strcmp(tdL.username, onlineUsers[i].username) == 0)
                {
                    printf("%d\n", onlineUsers[i].n);
                    onlineUsers[i].n--;
                    printf("%d\n", onlineUsers[i].n);
                    if (onlineUsers[i].n == 0)
                    {
                        time_t currentTime = time(NULL);
                        onlineUsers[i].lastConnectionTime = currentTime;

                        int tokeepSeconds = atoi(p.toKeep);

                        while (onlineUsers[i].n == 0)
                        {
                            sleep(1);
                            currentTime = time(NULL);
                            int elapsedSeconds = currentTime - onlineUsers[i].lastConnectionTime;

                            if (elapsedSeconds >= tokeepSeconds && onlineUsers[i].n == 0)
                            {
                                removeFromDb(tdL.username);
                                break;
                            }
                        }
                    }
                }
            }
            free((void *)tdL.username);
            break;
        }
        else if (tdL.connected == 1 && proxyCon == 1)
        {
            if (strncmp(msg, "USER", 4) == 0)
            {
                user(ftp, msg + 5, client);
                strcpy(tdL.username, msg + 5);
                tdL.username[strlen(tdL.username) - 1] = '\0';
                printf("%s\n", tdL.username);
            }
            else if (strncmp(msg, "PASS", 4) == 0)
            {
                pass(ftp, msg + 5, client, tdL.username);
                if (strcmp(tdL.username, "\0") != 0)
                {
                    printf("User connected: %s\n", tdL.username);
                    if (verify_if_user_exists(tdL.username) == 0)
                    {
                        add_user(tdL.username);
                        searchFiles(ftp, tdL.username, ".txt", "\0");
                        onlineUsers[nrOfOnlineUsers].n = 1;
                        strcpy(onlineUsers[nrOfOnlineUsers++].username, tdL.username);
                    }
                    else
                    {
                        for (int i = 0; i < nrOfOnlineUsers; i++)
                        {
                            if (strcmp(tdL.username, onlineUsers[i].username) == 0)
                            {
                                onlineUsers[i].n++;
                                break;
                            }
                        }
                    }
                }
                else
                {
                    printf("No user connected\n");
                }
            }
            else if (strncmp(msg, "RETR", 4) == 0)
            {
                printf("%s\n", tdL.username);
                msg[strlen(msg) - 1] = '\0';
                printf("%s\n", msg + 5);
                retr(client, msg + 5, tdL.username);
            }
            else if (strcmp(msg, "LIST\n") == 0)
            {
                printf("%s\n", tdL.username);
                msg[strlen(msg) - 1] = '\0';
                printf("%s\n", msg + 5);
                list(client, tdL.username);
            }
            else
            {
                printf("Unknown command: %s", msg);
                write(client, "Unknown command", strlen("Unknown command"));
            }
        }
        else if (proxyCon == 0 && (nrOf = sscanf(msg, "user %49s pass %49s\n", prUsr, passd)) == 2)
        {
            proxyCon = connToProxy(prUsr, passd, tdL.cl, &admin, tdL.proxyUser);
            printf("%d\n", proxyCon);
        }
        else if (proxyCon == 0 && tdL.connected == 0)
        {
            printf("You are not connected");
            write(client, "You are not connected", strlen("You are not connected"));
        }

        bzero(msg, sizeof(msg));
    }

    close(client);
    close(ftp);
}