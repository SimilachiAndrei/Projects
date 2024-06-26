function buildCompany() {             // Fetch company details for companies
    fetch('/api/company')
        .then(response => {
            if (!response.ok) {
                throw new Error('Network response was not ok');
            }
            return response.json();
        })
        .then(data => {
            const detailsContainer = document.createElement('div');
            detailsContainer.classList.add('middle');
            detailsContainer.innerHTML = `
                                <div id="company-details" class="content">
                                    <h1>Company Details:</h1>
                                    <div class="details-container">
                                        <div class="detail-item">
                                            <p><strong>Name:</strong> ${data.company.name || 'No name available'}</p>
                                            <form id="updateNameForm" method="PUT" action="/api/company/name">
                                                <label for="new-name">New Name:</label>
                                                <input type="text" id="new-name" name="new-name" required>
                                                <button type="submit">Save</button>
                                            </form>
                                        </div>
                                        <div class="detail-item">
                                            <p><strong>Address:</strong> ${data.company.address || 'No address available'}</p>
                                            <form id="updateAddressForm" method="PUT" action="/api/company/address">
                                                <label for="new-address">New Address:</label>
                                                <input type="text" id="new-address" name="new-address" required>
                                                <button type="submit">Save</button>
                                            </form>
                                        </div>
                                        <div class="detail-item">
                                            <p><strong>Phone:</strong> ${data.company.phone || 'No phone available'}</p>
                                            <form id="updatePhoneForm" method="PUT" action="/api/company/phone">
                                                <label for="new-phone">New Phone:</label>
                                                <input type="text" id="new-phone" name="new-phone" required>
                                                <button type="submit">Save</button>
                                            </form>
                                        </div>
                                        <div class="detail-item">
                                            <p><strong>Description:</strong> ${data.company.description || 'No description available'}</p>
                                            <form id="updateDescriptionForm" method="PUT" action="/api/company/description">
                                                <label for="new-description">New Description:</label>
                                                <textarea id="new-description" name="new-description" required></textarea>
                                                <button type="submit">Save</button>
                                            </form>
                                        </div>
                                        <div class="detail-item">
                                            <p><strong>Motto:</strong> ${data.company.motto || 'No motto available'}</p>
                                            <form id="updateMottoForm" method="PUT" action="/api/company/motto">
                                                <label for="new-motto">New Motto:</label>
                                                <input type="text" id="new-motto" name="new-motto" required>
                                                <button type="submit">Save</button>
                                            </form>
                                        </div>
                                        <div class="detail-item">
                                          <p><strong>Profile Picture:</strong></p>
                                          ${data.image && data.image.data ? `<img src="data:image/png;base64,${btoa(String.fromCharCode.apply(null, new Uint8Array(data.image.data.data)))}" alt="${data.image.name}" />` : '<p>No profile picture available</p>'}
                                          <form id="profilePictureForm">
                                            <input type="file" id="profilePictureInput" name="profilePicture" accept="image/*" required>
                                            <button type="submit">Update Profile Picture</button>
                                        </form>
                                      </div>
                                    </div>
                                </div>
                            `;
            document.querySelector('.container').insertBefore(detailsContainer, document.querySelector('.middle2'));
            setupCompanyFormListeners();
        })
        .catch(error => {
            console.error('Error fetching company details:', error);
            const errorContainer = document.createElement('div');
            errorContainer.classList.add('middle');
            errorContainer.innerHTML = `
                                <div id="company-details" class="content">
                                    <h1>Company Details:</h1>
                                    <div class="details-container">
                                        <p>Error fetching company details. Please try again later.</p>
                                    </div>
                                </div>
                            `;
            document.querySelector('.container').insertBefore(errorContainer, document.querySelector('.middle2'));
        });

}