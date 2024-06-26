document.addEventListener('DOMContentLoaded', () => {
    const urlParams = new URLSearchParams(window.location.search);
    const phaseId = urlParams.get('phaseId');

    document.getElementById('phaseId').value = phaseId;

    const form = document.getElementById('licitationForm');
    form.addEventListener('submit', async (event) => {
        event.preventDefault();

        const formData = new FormData(form);
        const data = {};
        formData.forEach((value, key) => {
            data[key] = value;
        });

        try {
            const response = await fetch('/api/addoffer', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                    'Authorization': `Bearer ${localStorage.getItem('jwtToken')}`
                },
                body: JSON.stringify(data)
            });
            if (response.ok) {
                console.log('Offer submitted successfully');
                // Redirect or display a success message
            } else {
                console.error('Failed to submit offer');
            }
        } catch (error) {
            console.error('Error submitting offer:', error);
        }
    });
});
