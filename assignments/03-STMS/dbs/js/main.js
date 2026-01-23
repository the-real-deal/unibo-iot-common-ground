const cssStyles = getComputedStyle(document.documentElement);
const color = cssStyles.getPropertyValue('--secondary-color').trim();

let waterLevels = [];
let timeStamps = [];
const nDataPoints = 10; 
let mode = "AUTOMATIC";

// We are using the library Chart.js to plot the water level graph
const canvas = document.getElementById('waterChart');
const ctx = canvas.getContext('2d');
const myChart = new Chart(ctx, {
    type: 'line', 
    data: {
        labels: timeStamps,
        datasets: [{
            label: "",
            data: waterLevels,
            borderColor: color,
            fill: true,
            backgroundColor: color + '50'
        }]
    },
    options: {
        plugins: {
            legend: { 
                display: false 
            }
        },
        responsive: true,
        scales: {
            y: {
                beginAtZero: true,
                max: 100,
                title: { display: true, text: 'water level (%)' }
            },
            x: {
                title: { display: true, text: 'time' }
            }
        }
    }
});

function updateUI() {
    const status = document.getElementById('status');
    const btn = document.getElementById('mode-button');
    const slider = document.getElementById('valve-slider');

    status.innerText = mode;
    
    if (mode === "AUTOMATIC") {
        btn.innerText = "Switch to MANUAL";
        slider.disabled = true;
    } else {
        btn.innerText = "Switch to AUTOMATIC";
        slider.disabled = false;
    }
}

function toggleMode() {
    mode = (mode === "AUTOMATIC") ? "MANUAL" : "AUTOMATIC";
    updateUI();

    console.log("Mode changed to:", mode);
}

function updateValveValue() {
    /*To do*/
}

updateUI();
simulateDBS();