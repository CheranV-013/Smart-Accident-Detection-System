const espIP = "http://172.20.10.2/data";
let alertShown = false;

function fetchData() {
  fetch(espIP)
    .then(res => res.json())
    .then(data => {

      const statusEl = document.getElementById("status");
      const circle = document.getElementById("statusCircle");
      const bar = document.getElementById("forceBar");

      // STATUS
      if (data.accident) {
        statusEl.innerText = "ACCIDENT 🚨";
        statusEl.className = "danger";
        circle.style.background = "#ef4444";
        circle.style.boxShadow = "0 0 15px #ef4444";

        if (!alertShown) {
          alert("🚨 ACCIDENT DETECTED!");
          alertShown = true;
        }

      } else {
        statusEl.innerText = "SAFE";
        statusEl.className = "safe";
        circle.style.background = "#22c55e";
        circle.style.boxShadow = "0 0 12px #22c55e";
        alertShown = false;
      }

      // FORCE
      let force = data.totalG.toFixed(2);
      document.getElementById("force").innerText = force + " g";

      let percent = Math.min(force * 20, 100);
      bar.style.width = percent + "%";

      if (percent > 70) {
        bar.style.background = "linear-gradient(90deg,#ef4444,#b91c1c)";
      } else {
        bar.style.background = "linear-gradient(90deg,#22c55e,#16a34a)";
      }

      // LOCATION
      document.getElementById("location").innerText =
        (data.lat === 0 && data.lng === 0)
          ? "GPS Not Fixed"
          : data.lat.toFixed(6) + ", " + data.lng.toFixed(6);

      // DIRECTION
      document.getElementById("direction").innerText =
        data.accident ? "Impact Detected" : "None";

    });
}

fetchData();
setInterval(fetchData, 1000);
