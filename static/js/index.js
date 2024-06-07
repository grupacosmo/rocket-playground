let data = {};

async function fetchData() {
  return JSON.parse(await (await fetch("/get")).json());
}

const defaultData = {
  gps: {
    latitude: 55.7506,
    longitude: 37.6175,
  },
};

function cloneObject(object) {
  return JSON.parse(JSON.stringify(object));
}

async function setData() {
  try {
    data = await fetchData();
    if (Object.keys(data).length === 0) {
      data = cloneObject(defaultData);
    }
  } catch {
    data = cloneObject(defaultData);
  }
}

async function init() {
  await setData();

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
    await setData();
    map.flyTo([data.gps.latitude, data.gps.longitude], 8);
    setMarkerLocation(data.gps.latitude, data.gps.longitude);
  }, 1500);
}

init();
