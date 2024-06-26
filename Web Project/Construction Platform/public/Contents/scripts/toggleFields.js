const clientRadio = document.querySelector('input[value="client"]');
const companyRadio = document.querySelector('input[value="company"]');
const companyFields = document.getElementById('companyFields');

clientRadio.addEventListener('change', function () {
  if (this.checked) {
    companyFields.style.display = 'none';
  }
});

companyRadio.addEventListener('change', function () {
  if (this.checked) {
    companyFields.style.display = 'block';
  }
});