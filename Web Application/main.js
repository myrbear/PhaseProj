/*******************************************************
 * TEMPORARY JS PHASE ENGINE (SIMULATES +MYRON API)
 * -----------------------------------------------------
 * Positions are stored as TOP-LEFT (like your objects).
 * Collisions are handled using circle bounds with radius r.
 *******************************************************/
const PhaseEngine = (() => {
  const state = {
    nextId: 1,
    bodies: new Map(), // id -> { id, x, y, vx, vy, r, mass }
    worldWidth: 800,
    worldHeight: 480,
    gravity: 300,    // pixels/sec^2 downward
    damping: 0.99,   // velocity damping
    restitution: 0.8 // bounciness
  };

  function init(width, height) {
    if (typeof width === "number") state.worldWidth = width;
    if (typeof height === "number") state.worldHeight = height;
  }

  function clear() {
    state.bodies.clear();
    state.nextId = 1;
  }

  function createObject(radius) {
    const id = state.nextId++;
    const r = radius || 20;
    state.bodies.set(id, {
      id,
      x: 0,
      y: 0,
      vx: 0,
      vy: 0,
      r,
      mass: Math.max(r, 1),
    });
    return id;
  }

  function setPosition(id, x, y) {
    const b = state.bodies.get(id);
    if (!b) return;
    b.x = x;
    b.y = y;
  }

  function setVelocity(id, vx, vy) {
    const b = state.bodies.get(id);
    if (!b) return;
    b.vx = vx;
    b.vy = vy;
  }

  function getPosX(id) {
    const b = state.bodies.get(id);
    return b ? b.x : 0;
  }

  function getPosY(id) {
    const b = state.bodies.get(id);
    return b ? b.y : 0;
  }

  function integrate(dt) {
    for (const b of state.bodies.values()) {
      // Gravity
      b.vy += state.gravity * dt;

      // Integrate velocity -> position
      b.x += b.vx * dt;
      b.y += b.vy * dt;

      // Damping
      b.vx *= state.damping;
      b.vy *= state.damping;

      // Wall collisions (treat as circle with center at x+r, y+r)
      const r = b.r;
      let cx = b.x + r;
      let cy = b.y + r;

      const left = r;
      const right = state.worldWidth - r;
      const top = r;
      const bottom = state.worldHeight - r;

      // Left / right
      if (cx < left) {
        cx = left;
        b.x = cx - r;
        b.vx = -b.vx * state.restitution;
      } else if (cx > right) {
        cx = right;
        b.x = cx - r;
        b.vx = -b.vx * state.restitution;
      }

      // Top / bottom
      if (cy < top) {
        cy = top;
        b.y = cy - r;
        b.vy = -b.vy * state.restitution;
      } else if (cy > bottom) {
        cy = bottom;
        b.y = cy - r;
        b.vy = -b.vy * state.restitution;
      }
    }
  }

  function resolveCollisions() {
    const bodies = Array.from(state.bodies.values());
    const e = state.restitution;

    for (let i = 0; i < bodies.length; ++i) {
      for (let j = i + 1; j < bodies.length; ++j) {
        const a = bodies[i];
        const b = bodies[j];

        const ax = a.x + a.r;
        const ay = a.y + a.r;
        const bx = b.x + b.r;
        const by = b.y + b.r;

        const dx = bx - ax;
        const dy = by - ay;
        const distSq = dx * dx + dy * dy;
        const minDist = a.r + b.r;
        const minDistSq = minDist * minDist;

        // No overlap or same position
        if (distSq >= minDistSq || distSq === 0) continue;

        const dist = Math.sqrt(distSq);
        const overlap = minDist - dist;

        const nx = dx / dist;
        const ny = dy / dist;

        const totalMass = a.mass + b.mass;
        const aShare = b.mass / totalMass;
        const bShare = a.mass / totalMass;

        // Separate bodies along normal
        const axNew = ax - nx * overlap * aShare;
        const ayNew = ay - ny * overlap * aShare;
        const bxNew = bx + nx * overlap * bShare;
        const byNew = by + ny * overlap * bShare;

        a.x = axNew - a.r;
        a.y = ayNew - a.r;
        b.x = bxNew - b.r;
        b.y = byNew - b.r;

        // Relative velocity along the normal
        const rvx = b.vx - a.vx;
        const rvy = b.vy - a.vy;
        const velAlongNormal = rvx * nx + rvy * ny;

        // Already separating
        if (velAlongNormal > 0) continue;

        // Impulse scalar
        const impulse =
          (-(1 + e) * velAlongNormal) * (1 / (1 / a.mass + 1 / b.mass));
        const ix = impulse * nx;
        const iy = impulse * ny;

        a.vx -= ix / a.mass;
        a.vy -= iy / a.mass;
        b.vx += ix / b.mass;
        b.vy += iy / b.mass;
      }
    }
  }

  function step(dt) {
    if (!dt || dt <= 0) return;
    integrate(dt);
    resolveCollisions();
  }

  return {
    init,
    clear,
    createObject,
    setPosition,
    setVelocity,
    getPosX,
    getPosY,
    step,
  };
})();

/*******************************************************
 * YOUR ORIGINAL UI / EDITOR CODE, WIRED TO PhaseEngine
 *******************************************************/

// Canvas setup
const canvas = document.getElementById("canvas");
const ctx = canvas.getContext("2d");

const WIDTH = canvas.width;   // logical width (800)
const HEIGHT = canvas.height; // logical height (480);

// Initialize engine with canvas size
PhaseEngine.init(WIDTH, HEIGHT);

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
// {
//   id,
//   engineId,
//   position: {x, y},
//   radius,
//   color,
//   shapeType,
//   isSelected
// }

let nextObjectId = 1;
function createObject(x, y, r, color, shapeType) {
  const randomAngle = Math.random() * Math.PI * 2;
  const speed = 40 + Math.random() * 60; // px/sec

  const vx = Math.cos(randomAngle) * speed;
  const vy = Math.sin(randomAngle) * speed;

  // "Call" the engine (this stands in for Tyler/Myron C++ side)
  const engineId = PhaseEngine.createObject(r);
  PhaseEngine.setPosition(engineId, x, y);
  PhaseEngine.setVelocity(engineId, vx, vy);

  return {
    id: nextObjectId++,
    engineId,
    position: { x, y }, // top-left
    radius: r,
    color,
    shapeType,
    isSelected: false,
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

  // Keep engine in sync when user edits properties
  if (activeObject.engineId != null) {
    PhaseEngine.setPosition(activeObject.engineId, activeObject.position.x, activeObject.position.y);
  }
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
  PhaseEngine.clear();
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

    // Keep engine in sync while dragging
    if (activeObject.engineId != null) {
      PhaseEngine.setPosition(
        activeObject.engineId,
        activeObject.position.x,
        activeObject.position.y
      );
    }
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

// Simulation step (calls PhaseEngine)
function stepSimulation(dtSeconds) {
  if (!simulationRunning) return;

  const dt = Math.min(dtSeconds, 0.05); // clamp dt for stability

  PhaseEngine.step(dt);

  // Sync positions from engine back into render objects
  objects.forEach((obj) => {
    if (obj.engineId != null) {
      obj.position.x = PhaseEngine.getPosX(obj.engineId);
      obj.position.y = PhaseEngine.getPosY(obj.engineId);
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
