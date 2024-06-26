const reviews = document.querySelectorAll('.review');
const prevReviewBtn = document.querySelector('.prev-review');
const nextReviewBtn = document.querySelector('.next-review');
let currentReview = 0;

function showReview(index) {
    reviews[currentReview].classList.remove('active');
    reviews[index].classList.add('active');
    currentReview = index;
}

function showNextReview() {
    let nextIndex = (currentReview + 1) % reviews.length;
    showReview(nextIndex);
}

function showPrevReview() {
    let prevIndex = (currentReview - 1 + reviews.length) % reviews.length;
    showReview(prevIndex);
}

prevReviewBtn.addEventListener('click', showPrevReview);
nextReviewBtn.addEventListener('click', showNextReview);
