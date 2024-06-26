function buildClient() {      
    // Fetch posts for clients
    const xhr = new XMLHttpRequest();
    xhr.open('GET', '/api/user-data', true);

    xhr.onload = function () {
        if (xhr.status === 200) {
            const data = JSON.parse(xhr.responseText);
            const postContainer = document.createElement('div');
            postContainer.classList.add('middle');
            postContainer.innerHTML = `
                                <div id="client-posts" class="content">
                                    <h1>Your Posts:</h1>
                                    <div class="post-container"></div>
                                </div>
                            `;
            document.querySelector('.container').insertBefore(postContainer, document.querySelector('.middle2'));
            const clientPosts = document.getElementById('client-posts');
            const postsContainer = clientPosts.querySelector('.post-container');
            postsContainer.innerHTML = '';

            if (!data.projects || data.projects.length === 0) {
                clientPosts.style.display = 'none';
            } else {
                clientPosts.style.display = 'block';
                data.projects.forEach(project => {
                    if (project.name !== null) {
                        const projectName = project.name || 'No name provided';
                        const projectDescription = project.description || 'No description provided';
                        const projectStatus = project.phases && project.phases.every(phase => (phase.state == 'finished' || phase.state == 'reviewed')) ? 'finished' : 'in progress';
                        const projectPhases = project.phases || [];

                        const projectItem = document.createElement('div');
                        projectItem.classList.add('post-item');
                        projectItem.innerHTML = `
                                            <h3>${projectName}</h3>
                                            <h4>Status: ${projectStatus}</h4>
                                            <div class="description">
                                                <strong>Description:</strong>
                                                <div class="descontent">${projectDescription}</div>
                                                <strong>Phases:</strong>
                                                <ul>
                                                    ${projectPhases.map(phase => `
                                                        <li>
                                                            ${phase.description || 'No phase description provided'}
                                                            ${phase.company ? `<span>Company: ${phase.company.name}</span>` : ''}
                                                            ${phase.state === "notpending" ? `<button data-phase-id="${phase.id}">Licitate</button>` : ''}
                                                            </br>
                                                            ${(phase.state === "finished" || phase.state === "reviewed") ? `
                                                            ${phase.images.length > 0 ? `<img src="data:image/jpeg;base64,${btoa(String.fromCharCode(...new Uint8Array(phase.images[0].data.data)))}" alt="Phase Image" />` : ''}
                                                            ` : ''}
                                                        </li>
                                                    `).join('')}
                                                </ul>
                                            </div>
                                        `;

                        postsContainer.appendChild(projectItem);
                    }
                });

                // Move the event listener setup here
                const licitateButtons = document.querySelectorAll('button[data-phase-id]');
                licitateButtons.forEach(button => {
                    button.addEventListener('click', async () => {
                        const phaseId = button.getAttribute('data-phase-id');
                        console.log(phaseId);
                        try {
                            const response = await fetch('/api/addlicitation', {
                                method: 'POST',
                                headers: {
                                    'Content-Type': 'application/json'
                                },
                                body: JSON.stringify({ phaseId })
                            });
                            if (response.ok) {
                                console.log('Licitation added successfully');
                                location.reload();
                            } else {
                                console.error('Failed to add licitation');
                                location.reload();
                            }
                        } catch (error) {
                            console.error('Error adding licitation:', error);
                        }
                    });
                });
            }
        } else {
            console.error('Error fetching user data:', xhr.status);
        }
    };

    xhr.onerror = function () {
        console.error('Error fetching user data');
    };

    xhr.send();
}