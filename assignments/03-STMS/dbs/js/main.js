let currentValveOpening = 0;
let currentWaterLevel = 0;
const nDataPoints = CONFIG.MAX_SAMPLES; 
let mode = "NOT_AVAILABLE";
let waterChart;

document.addEventListener("DOMContentLoaded", function(){
    const cssStyles = getComputedStyle(document.documentElement);
    const color = cssStyles.getPropertyValue('--secondary-color').trim();
    
    const canvas = document.getElementById('waterChart');
    const ctx = canvas.getContext('2d');
    
    //inizializzo la chart
    waterChart = new Chart(ctx, {
        type: 'line', 
        data: {
            labels: [],
            datasets: [{
                label: "Water Level",
                data: [],
                borderColor: color,
                fill: true,
                backgroundColor: color + '50',
            }]
        },
        options: {
            plugins: { legend: { display: false } },
            responsive: true,
            scales: {
                y: {
                    beginAtZero: true,
                    max: 100,
                    title: { display: true, text: 'water level (%)' }
                },
                x: { title: { display: true, text: 'time' } }
            }
        }
    });

    const modeBtn = document.getElementById('mode-button');
    modeBtn.addEventListener('click', toggleMode);
    
    const slider = document.getElementById('valve-slider');
    slider.addEventListener('change', () => updateValveValue(slider.value, mode)); //value viene aggiornato per la funzione sotto
    console.log(slider.value);
    slider.addEventListener('change', function() {
        const valueDisplay = document.getElementById('valve');
        if (valueDisplay) {
            valueDisplay.innerText = slider.value + '%';
        }
    });

    updateUI();
    CUSpolling();
});

async function toggleMode() {
    if(mode === 'UNCONNECTED' || mode === 'NOT_AVAILABLE'){
        return;
    };
    const newMode = (mode === 'AUTOMATIC') ? 'MANUAL' : 'AUTOMATIC';
    await API.setMode(newMode);
    mode = newMode;
    updateUI();
}

async function updateDashboard() {
    try {
        const statusData = await API.getStatus();
        const waterData = await API.getWaterLevel();
        const valveData = await API.getValveOpening(); 

        mode = statusData.status || statusData.mode; 
        currentWaterLevel = waterData.level;
        currentValveOpening = valveData.opening * 100; //per il display della percentuale

        updateUI();
        updateChart(waterData);
    } catch (err) {
        console.error("Polling error:", err);
    }
}

async function CUSpolling(){
       setInterval(() => {
       updateDashboard();
    }, CONFIG.POLL_INTERVAL);
}

function updateChart(waterData) {
    if (!waterChart || waterData.level === undefined) return;
    const totalSeconds = Math.floor(waterData.timestampMS / 1000);
    const minutes = Math.floor(totalSeconds / 60);
    const seconds = totalSeconds % 60;
    const timeLabel = `${minutes}:${seconds.toString().padStart(2, '0')}`;

    waterChart.data.labels.push(timeLabel);
    waterChart.data.datasets[0].data.push(waterData.level);

    while (waterChart.data.labels.length > nDataPoints) {
        waterChart.data.labels.shift();
        waterChart.data.datasets[0].data.shift();
    }
    waterChart.update();
}

function updateUI() {
    //funzione per l'aggiornamento dei componenti come lo span dello status, lo slider e il bottone dello switch mode
    const status = document.getElementById('status');
    const valveValue = document.getElementById('valve');
    const btn = document.getElementById('mode-button');
    const slider = document.getElementById('valve-slider');
    status.innerText = mode.replace('_', ' ');

    if (mode === "AUTOMATIC") {
        btn.innerText = "Switch to MANUAL";
        btn.disabled = false;
        slider.disabled = true;
    } else if (mode === "MANUAL") {
        console.log("should be manual");
        btn.innerText = "Switch to AUTOMATIC";
        btn.disabled = false;
        slider.disabled = false;
    } else {
        btn.disabled = true;
        slider.disabled = true;
    }
    
    if (valveValue) valveValue.textContent = Math.round(currentValveOpening) + '%';
    if (slider) slider.value = currentValveOpening;
}

async function updateValveValue(valoreSlider, mode) {
    if(!valoreSlider){
        return;
    }
    if (isNaN(valoreSlider)){
        return;
    }
    const value = valoreSlider/100;
    console.log(value);
        if (mode !== 'MANUAL') return;

        if(value > 1 || value < 0) {
            console.log("Valori non validi");
            return; // valori non validi
        }
        try {
            await API.setValveOpening(value);
            currentValveOpening = value;
        } catch (error) {
            console.error("Error setting valve:", error);
        }
}