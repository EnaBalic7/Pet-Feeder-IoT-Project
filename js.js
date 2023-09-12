const firebaseConfig = {
    apiKey: "YOUR_API_KEY",
    authDomain: "YOUR_AUTH_DOMAIN",
    databaseURL: "YOUR_DATABASE_URL",
    projectId: "YOUR_PROJECT_ID",
    storageBucket: "YOUR_STORAGE_BUCKET",
    messagingSenderId: "YOUR_MESSAGING_SENDER_ID",
    appId: "YOUR_APP_ID",
    measurementId: "YOUR_MEASUREMENT_ID"
};

firebase.initializeApp(firebaseConfig);
firebase.analytics();

const distanceRef = firebase.database().ref("distance");
const petImage = document.getElementById("pet");
const reminderSound = document.getElementById("reminderSound");
const foodLowRef = firebase.database().ref("foodLow");

foodLowRef.on("value", (snapshot)=>{
  const foodLowValue = snapshot.val();

  if (foodLowValue == 1) {
    playReminderSound();
    showFoodReminder();
  } else {
    hideFoodReminder();
  }

});

distanceRef.on("value", (snapshot) => {
  const distance = snapshot.val(); 
  updateDistanceUI(distance);
  updateFoodProgress(distance);
  updatePetImage(distance);

});

function playReminderSound() {
  if (reminderSound) {
    reminderSound.play();
  }
}

function updateDistanceUI(distance) {
  const distanceElement = document.getElementById("distance"); 
  if (distanceElement) {
    distanceElement.textContent = `Distance: ${distance} cm`;
  }
}

function updateFoodProgress(distance) {
  const foodProgress = document.getElementById("foodProgress");
  if (foodProgress) {
    const maxDistance = 20;
    const remainingDistance = Math.max(0, maxDistance - distance);
    const percentageLeft = (remainingDistance / maxDistance) * 100;
    foodProgress.style.width = `${percentageLeft}%`;
    document.getElementById("fullness").innerHTML = `${percentageLeft.toFixed(0)}%`
  }
}

function updatePetImage(distance) {
  if (petImage) {
    if (distance > 14) {
      petImage.src = "sadKitty.png";
    }
    else if(distance <= 14 && distance > 5) {
      petImage.src = "happyKitty.png"; 
    }
    else{
      petImage.src = "eatingKitty.png";
    }
  }
}

function showFoodReminder() {
  const reminderPopup = document.getElementById("reminderPopup");
  const reminderPopupBg = document.getElementById("bg");
  if (reminderPopup && reminderPopupBg) {
    reminderPopup.style.display = "flex";
    reminderPopupBg.style.display = "flex";
  }
}

function hideFoodReminder() {
  const reminderPopup = document.getElementById("reminderPopup");
  const reminderPopupBg = document.getElementById("bg");
  if (reminderPopup && reminderPopupBg) {
    reminderPopup.style.display = "none";
    reminderPopupBg.style.display = "none";
  }
}


const closeReminderBtn = document.getElementById("closeReminder");
if (closeReminderBtn) {
  closeReminderBtn.addEventListener("click", hideFoodReminder);
}

function dispenseFood(){
  const dispense = firebase.database().ref("dispense");
  dispense.set(1).then(() => {
    console.log('Dispense successfully updated to value 1');
  }).catch((error) => {
    console.error('Error updating value:', error);
  });
}


var foodDispensed = firebase.database().ref('foodDispensed');

foodDispensed.on('value', function(snapshot) {
  var foodDispensedValue = snapshot.val();
  
  if (foodDispensedValue === 1) {
    
    var dispenseMessage = document.getElementById("dispense-msg");
    dispenseMessage.innerHTML = "Food dispensed successfully!";
    dispenseMessage.style.display = "flex";

    
    setTimeout(function() {
      dispenseMessage.style.display = "none";
    }, 3000);

    
    foodDispensed.set(0);
  }
});