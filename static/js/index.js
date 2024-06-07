let data = {};

async function fetchData() {
  return JSON.parse(await (await fetch("/get")).json());
}

async function init() {
  data = await fetchData();

  const map = L.map("map").setView([data.gps.latitude, data.gps.longitude], 13);

  L.tileLayer("https://tile.openstreetmap.org/{z}/{x}/{y}.png", {
    maxZoom: 19,
    attribution:
      '&copy; <a href="http://www.openstreetmap.org/copyright">OpenStreetMap</a>',
  }).addTo(map);

  const marker = L.marker([data.gps.latitude, data.gps.longitude]).addTo(map);

  function setMarkerLocation(lat, lng) {
    const newLoc = new L.LatLng(lat, lng);
    marker.setLatLng(newLoc);
  }

  setInterval(async () => {
    data = await fetchData();
    map.flyTo([data.gps.latitude, data.gps.longitude], 8);
    setMarkerLocation(data.gps.latitude, data.gps.longitude);
  }, 1500);
}

init();
