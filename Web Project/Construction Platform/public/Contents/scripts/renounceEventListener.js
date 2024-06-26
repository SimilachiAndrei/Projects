document.getElementById('renounceProjectButton').addEventListener('click', function() {
    var phaseId = document.getElementById('phaseId').value;
    var jwtToken = localStorage.getItem('jwtToken');

    if (phaseId) {
        fetch(`/api/project/phases/${phaseId}`, {
            method: 'DELETE',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': `Bearer ${jwtToken}`
            },
        })
        .then(response => {
            if (response.ok) {
                alert('Project phase renounced successfully');
                location.reload();
            } else {
                alert('Failed to renounce project phase');
            }
        })
        .catch(error => console.error('Error:', error));
    }
});
