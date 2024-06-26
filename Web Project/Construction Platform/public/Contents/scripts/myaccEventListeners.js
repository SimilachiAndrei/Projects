function setupCompanyFormListeners() {
    document.getElementById('updateMottoForm').addEventListener('submit', function (event) {
        event.preventDefault();

        const formData = new FormData(this);
        const motto = formData.get('new-motto');
        console.log(motto)
        const token = localStorage.getItem('jwtToken');

        fetch('/api/company/motto', {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': `Bearer ${token}`
            },
            body: JSON.stringify({ motto })
        })
            .then(response => {
                if (!response.ok) {
                    throw new Error('Network response was not ok');
                }
                return response.json();
            })
            .then(data => {
                console.log('PUT request successful:', data);
                location.reload();
            })
            .catch(error => {
                console.error('Error in PUT request:', error);
                location.reload();
            });
    });


    document.getElementById('profilePictureForm').addEventListener('submit', function (event) {
        event.preventDefault();
        const form = document.getElementById('profilePictureForm');
        const fileInput = document.getElementById('profilePictureInput');

        form.addEventListener('submit', async (event) => {
            event.preventDefault();

            const file = fileInput.files[0];
            const formData = new FormData();
            formData.append('profilePicture', file);

            try {
                const response = await fetch('/api/company/profile-picture', {
                    method: 'PUT',
                    body: formData
                });

                if (response.ok) {
                    console.log('Profile picture updated successfully');
                    location.reload();

                } else {
                    console.error('Error updating profile picture:', response.statusText);
                    location.reload();

                }
            } catch (error) {
                console.error('Error updating profile picture:', error);
                location.reload();

            }
        });
    });


    document.getElementById('updateDescriptionForm').addEventListener('submit', function (event) {
        event.preventDefault();

        const formData = new FormData(this);
        const description = formData.get('new-description');
        console.log(description)
        const token = localStorage.getItem('jwtToken');

        fetch('/api/company/description', {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': `Bearer ${token}`
            },
            body: JSON.stringify({ description })
        })
            .then(response => {
                if (!response.ok) {
                    throw new Error('Network response was not ok');
                }
                return response.json();
            })
            .then(data => {
                console.log('PUT request successful:', data);
                location.reload();
            })
            .catch(error => {
                console.error('Error in PUT request:', error);
                location.reload();
            });
    });


    document.getElementById('updatePhoneForm').addEventListener('submit', function (event) {
        event.preventDefault();

        const formData = new FormData(this);
        const phone = formData.get('new-phone');
        console.log(phone)
        const token = localStorage.getItem('jwtToken');

        fetch('/api/company/phone', {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': `Bearer ${token}`
            },
            body: JSON.stringify({ phone })
        })
            .then(response => {
                if (!response.ok) {
                    throw new Error('Network response was not ok');
                }
                return response.json();
            })
            .then(data => {
                console.log('PUT request successful:', data);
                location.reload();
            })
            .catch(error => {
                console.error('Error in PUT request:', error);
                location.reload();
            });
    });


    document.getElementById('updateAddressForm').addEventListener('submit', function (event) {
        event.preventDefault();

        const formData = new FormData(this);
        const address = formData.get('new-address');
        console.log(address)
        const token = localStorage.getItem('jwtToken');

        fetch('/api/company/address', {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': `Bearer ${token}`
            },
            body: JSON.stringify({ address })
        })
            .then(response => {
                if (!response.ok) {
                    throw new Error('Network response was not ok');
                }
                return response.json();
            })
            .then(data => {
                console.log('PUT request successful:', data);
                location.reload();
            })
            .catch(error => {
                console.error('Error in PUT request:', error);
                location.reload();
            });
    });

    document.getElementById('updateNameForm').addEventListener('submit', function (event) {
        event.preventDefault();

        const formData = new FormData(this);
        const name = formData.get('new-name');
        console.log(name)
        const token = localStorage.getItem('jwtToken');

        fetch('/api/company/name', {
            method: 'PUT',
            headers: {
                'Content-Type': 'application/json',
                'Authorization': `Bearer ${token}`
            },
            body: JSON.stringify({ name })
        })
            .then(response => {
                if (!response.ok) {
                    throw new Error('Network response was not ok');
                }
                return response.json();
            })
            .then(data => {
                console.log('PUT request successful:', data);
                location.reload();
            })
            .catch(error => {
                console.error('Error in PUT request:', error);
                location.reload();
            });
    });
}

