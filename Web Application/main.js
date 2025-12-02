// Canvas setup
const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");

const WIDTH = canvas.width;   // logical width (800)
const HEIGHT = canvas.height; // logical height (480)

// UI elements
const toolButtons = document.querySelectorAll(".tool-button[data-shape]");
const addObjectBtn = document.getElementById("add-object-btn");
const objectCountLabel = document.getElementById("object-count-label");
const statusLabel = document.getElementById("status-label");
const mouseLabel = document.getElementById("mouse-label");

const inspectorEmpty = document.getElementById("inspector-empty");
const inspectorFields = document.getElementById("inspector-fields");
const fieldX = document.getElementById("field-x");
const fieldY = document.getElementById("field-y");
const fieldRadius = document.getElementById("field-radius");
const fieldColor = document.getElementById("field-color");
const clearSceneBtn = document.getElementById("clear-scene-btn");

const playPauseBtn = document.getElementById("play-pause-btn");
const simStatusPill = document.getElementById("sim-status-pill");

// Scene state
let objects = [];
let selectedShapeType = "circle";

let activeObject = null;
let isDragging = false;
let dragOffset = { x: 0, y: 0 };

let simulationRunning = false;
let lastTime = performance.now();

// Object structure:
// { id, position: {x, y}, radius, color, shapeType, isSelected, velocity: {vx, vy} }

let nextObjectId = 1;
function createObject(x, y, r, color, shapeType) {
  const randomAngle = Math.random() * Math.PI * 2;
  const speed = 40 + Math.random() * 60; // px/sec

  return {
    id: nextObjectId++,
    position: { x, y },
    radius: r,
    color,
    shapeType,
    isSelected: false,
    velocity: {
      vx: Math.cos(randomAngle) * speed,
      vy: Math.sin(randomAngle) * speed,
    },
  };
}

function updateObjectCount() {
  objectCountLabel.textContent =
    objects.length === 1 ? "1 object" : `${objects.length} objects`;
}

function setStatus(text) {
  statusLabel.textContent = text;
}

function setMouseLabel(x, y) {
  mouseLabel.textContent = `Mouse: ${x.toFixed(0)}, ${y.toFixed(0)}`;
}

// Hit test (circle around center)
function hitTest(obj, point) {
  const cx = obj.position.x + obj.radius;
  const cy = obj.position.y + obj.radius;
  const dx = point.x - cx;
  const dy = point.y - cy;
  return dx * dx + dy * dy <= obj.radius * obj.radius;
}

// Selection & inspector
function selectObject(obj) {
  objects.forEach((o) => (o.isSelected = false));
  activeObject = obj;
  if (obj) {
    obj.isSelected = true;
    inspectorEmpty.classList.add("hidden");
    inspectorFields.classList.remove("hidden");
    fieldX.value = Math.round(obj.position.x);
    fieldY.value = Math.round(obj.position.y);
    fieldRadius.value = Math.round(obj.radius);
    fieldColor.value = rgbToHex(obj.color);
    setStatus(`Selected object #${obj.id}`);
  } else {
    inspectorEmpty.classList.remove("hidden");
    inspectorFields.classList.add("hidden");
    setStatus("Ready");
  }
}

// Convert CSS rgb(...) or named color to hex, fallback to white
function rgbToHex(color) {
  if (color && color.startsWith && color.startsWith("#")) return color;
  return "#ffffff";
}

// Apply inspector fields back to object
function applyInspectorChanges() {
  if (!activeObject) return;

  const x = parseFloat(fieldX.value);
  const y = parseFloat(fieldY.value);
  const r = parseFloat(fieldRadius.value);
  const c = fieldColor.value;

  if (!Number.isNaN(x)) activeObject.position.x = x;
  if (!Number.isNaN(y)) activeObject.position.y = y;
  if (!Number.isNaN(r) && r > 0) activeObject.radius = r;
  activeObject.color = c;
}

// Tool selection
toolButtons.forEach((btn) => {
  btn.addEventListener("click", () => {
    const shape = btn.dataset.shape;
    selectedShapeType = shape;
    toolButtons.forEach((b) => b.classList.remove("active"));
    btn.classList.add("active");
    setStatus(`Shape tool: ${shape}`);
  });
});

if (toolButtons.length > 0) {
  toolButtons[0].classList.add("active");
}

// Add object
addObjectBtn.addEventListener("click", () => {
  const pos = { x: 200, y: 200 };
  const r = 36;
  let color = "#f97316"; // orange
  if (selectedShapeType === "square") color = "#22c55e"; // green
  if (selectedShapeType === "triangle") color = "#3b82f6"; // blue;

  const obj = createObject(pos.x, pos.y, r, color, selectedShapeType);
  objects.push(obj);
  updateObjectCount();
  selectObject(obj);
});

// Clear scene
clearSceneBtn.addEventListener("click", () => {
  objects = [];
  updateObjectCount();
  selectObject(null);
});

// Play / Pause simulation
function updateSimUI() {
  if (simulationRunning) {
    playPauseBtn.textContent = "⏸ Pause";
    simStatusPill.textContent = "Playing";
    simStatusPill.classList.remove("pill-idle");
    simStatusPill.classList.add("pill-running");
    setStatus("Simulation running");
  } else {
    playPauseBtn.textContent = "▶ Play";
    simStatusPill.textContent = "Paused";
    simStatusPill.classList.remove("pill-running");
    simStatusPill.classList.add("pill-idle");
    setStatus("Simulation paused");
  }
}

playPauseBtn.addEventListener("click", () => {
  simulationRunning = !simulationRunning;
  updateSimUI();
});

// Canvas interactions
function getMousePos(evt) {
  const rect = canvas.getBoundingClientRect();
  // Convert from CSS pixels to canvas logical coordinates
  const scaleX = WIDTH / rect.width;
  const scaleY = HEIGHT / rect.height;
  return {
    x: (evt.clientX - rect.left) * scaleX,
    y: (evt.clientY - rect.top) * scaleY,
  };
}

canvas.addEventListener("mousedown", (e) => {
  const mouse = getMousePos(e);
  setMouseLabel(mouse.x, mouse.y);

  let clickedObj = null;
  for (let i = objects.length - 1; i >= 0; --i) {
    if (hitTest(objects[i], mouse)) {
      clickedObj = objects[i];
      break;
    }
  }

  if (clickedObj) {
    selectObject(clickedObj);
    isDragging = true;
    const cx = clickedObj.position.x + clickedObj.radius;
    const cy = clickedObj.position.y + clickedObj.radius;
    dragOffset.x = mouse.x - cx;
    dragOffset.y = mouse.y - cy;
    setStatus(`Dragging object #${clickedObj.id}`);
  } else {
    selectObject(null);
  }
});

canvas.addEventListener("mousemove", (e) => {
  const mouse = getMousePos(e);
  setMouseLabel(mouse.x, mouse.y);

  if (isDragging && activeObject) {
    const newCenterX = mouse.x - dragOffset.x;
    const newCenterY = mouse.y - dragOffset.y;
    activeObject.position.x = newCenterX - activeObject.radius;
    activeObject.position.y = newCenterY - activeObject.radius;
  }
});

canvas.addEventListener("mouseup", () => {
  isDragging = false;
  if (activeObject) {
    setStatus(`Selected object #${activeObject.id}`);
  } else {
    setStatus("Ready");
  }
});

canvas.addEventListener("mouseleave", () => {
  isDragging = false;
});

// Inspector field events
[fieldX, fieldY, fieldRadius, fieldColor].forEach((input) => {
  input.addEventListener("change", () => {
    applyInspectorChanges();
  });
});

// Simulation step (placeholder for PHASE engine)
function stepSimulation(dtSeconds) {
  if (!simulationRunning) return;

  const dt = Math.min(dtSeconds, 0.05); // clamp dt

  objects.forEach((obj) => {
    obj.position.x += obj.velocity.vx * dt;
    obj.position.y += obj.velocity.vy * dt;

    const r = obj.radius;

    // Bounce on bounds
    if (obj.position.x < 0) {
      obj.position.x = 0;
      obj.velocity.vx *= -1;
    } else if (obj.position.x + 2 * r > WIDTH) {
      obj.position.x = WIDTH - 2 * r;
      obj.velocity.vx *= -1;
    }

    if (obj.position.y < 0) {
      obj.position.y = 0;
      obj.velocity.vy *= -1;
    } else if (obj.position.y + 2 * r > HEIGHT) {
      obj.position.y = HEIGHT - 2 * r;
      obj.velocity.vy *= -1;
    }
  });
}

// Drawing
function drawObject(obj) {
  ctx.save();
  ctx.fillStyle = obj.color;

  if (obj.isSelected) {
    ctx.shadowColor = "rgba(59,130,246,0.8)";
    ctx.shadowBlur = 16;
  } else {
    ctx.shadowBlur = 0;
  }

  const x = obj.position.x;
  const y = obj.position.y;
  const r = obj.radius;

  if (obj.shapeType === "circle") {
    const cx = x + r;
    const cy = y + r;
    ctx.beginPath();
    ctx.arc(cx, cy, r, 0, Math.PI * 2);
    ctx.fill();
  } else if (obj.shapeType === "square") {
    ctx.beginPath();
    if (ctx.roundRect) {
      ctx.roundRect(x, y, r * 2, r * 2, 6);
    } else {
      ctx.rect(x, y, r * 2, r * 2);
    }
    ctx.fill();
  } else if (obj.shapeType === "triangle") {
    ctx.beginPath();
    ctx.moveTo(x + r, y);
    ctx.lineTo(x + r * 2, y + r * 2);
    ctx.lineTo(x, y + r * 2);
    ctx.closePath();
    ctx.fill();
  }

  ctx.restore();
}

function renderFrame(timestamp) {
  const dtSeconds = (timestamp - lastTime) / 1000.0;
  lastTime = timestamp;

  stepSimulation(dtSeconds);

  // Background
  const gradient = ctx.createLinearGradient(0, 0, 0, HEIGHT);
  gradient.addColorStop(0, "#020617");
  gradient.addColorStop(0.4, "#020617");
  gradient.addColorStop(1, "#020617");
  ctx.fillStyle = gradient;
  ctx.fillRect(0, 0, WIDTH, HEIGHT);

  // Objects
  objects.forEach((obj) => drawObject(obj));

  requestAnimationFrame(renderFrame);
}

// ---------- Initial setup ----------

// Default objects so you always see something
objects.push(createObject(150, 150, 40, "#f97316", "circle"));
objects.push(createObject(350, 220, 30, "#22c55e", "square"));
objects.push(createObject(550, 120, 35, "#3b82f6", "triangle"));

updateObjectCount();
selectObject(null);
setStatus("Ready");
setMouseLabel(0, 0);
updateSimUI();

// Kick off render loop
requestAnimationFrame((t) => {
  lastTime = t;
  renderFrame(t);
});
