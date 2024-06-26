document.addEventListener('DOMContentLoaded', async () => {
    try {
        const response = await fetch('/api/licitations'); // Fetch data from your API endpoint
        if (!response.ok) {
            throw new Error('Failed to fetch data');
        }
        const data = await response.json(); // Parse response JSON

        const licitationList = document.getElementById('licitationList');
        licitationList.innerHTML = ''; // Clear previous licitations

        data.forEach(licitation => {
            const licitationItem = document.createElement('div');
            licitationItem.classList.add('licitation-item');
            licitationItem.innerHTML = `
                <h3>Phase: ${licitation.id}</h3>
                <h1>User: ${licitation.username}</h1>
                <p>Description: ${licitation.description}</p>
                <button data-phase-id="${licitation.id}" class="licite-button">Licitate</button>
            `;
            licitationList.appendChild(licitationItem);
        });

        // Add event listeners to the dynamically created buttons
        document.querySelectorAll('button[data-phase-id]').forEach(button => {
            button.addEventListener('click', () => {
                const phaseId = button.getAttribute('data-phase-id');
                const encodedPhaseId = encodeURIComponent(phaseId); // Encode to prevent XSS
                window.location.href = `/licitationForm?phaseId=${encodedPhaseId}`;
            });
        });

    } catch (error) {
        console.error('Error fetching data:', error);
    }
});
