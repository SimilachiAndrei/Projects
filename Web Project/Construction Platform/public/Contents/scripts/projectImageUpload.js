document.addEventListener('DOMContentLoaded', async () => {
    try {
        // Fetch authorization token from localStorage
        const jwtToken = localStorage.getItem('jwtToken');

        // Fetch projects with phase details
        const response = await fetch('/api/projects', {
            headers: {
                'Authorization': `Bearer ${jwtToken}`
            }
        });

        if (!response.ok) {
            throw new Error(`Failed to fetch projects: ${response.status}`);
        }

        const projects = await response.json();

        // Display project details
        const projectDetailsContainer = document.querySelector('.project-container');
        projectDetailsContainer.innerHTML = '';

        projects.forEach(project => {
            const projectItem = document.createElement('div');
            projectItem.classList.add('project-item');
            projectItem.innerHTML = `
                <h3>Phase ID: ${project.id}</h3>
                <p>Description: ${project.description}</p>
            `;
            projectDetailsContainer.appendChild(projectItem);

            // Populate phase ID options in the upload form
            const phaseSelect = document.getElementById('phaseId');
            const option = document.createElement('option');
            option.value = project.id; // Use 'project.id' instead of 'project.phase_id'
            option.textContent = `Phase ID: ${project.id}`;
            phaseSelect.appendChild(option);
        });

        // Handle image upload form submission
        const imageUploadForm = document.getElementById('imageUploadForm');
        imageUploadForm.addEventListener('submit', async (event) => {
            event.preventDefault();

            const formData = new FormData();
            formData.append('phaseId', imageUploadForm.elements.phaseId.value);
            formData.append('phasePicture', imageUploadForm.elements.imageFile.files[0]);

            try {
                const uploadResponse = await fetch('/api/uploadImage', {
                    method: 'PUT',
                    headers: {
                        'Authorization': `Bearer ${jwtToken}`
                    },
                    body: formData
                });

                if (uploadResponse.ok) {
                    console.log('Image uploaded successfully');
                    location.reload();
                } else {
                    console.error('Failed to upload image');
                    location.reload();
                }
            } catch (error) {
                console.error('Error uploading image:', error);
                location.reload();
            }
        });

    } catch (error) {
        console.error('Error fetching projects:', error);
    }
});
