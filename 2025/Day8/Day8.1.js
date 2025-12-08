/**
 * Advent of Code 2025 - Day 8, Part 1
 * 
 * Description: https://adventofcode.com/2025/day/8
 * Challenge: Connect junction boxes with minimum string lights to form circuits
 * Algorithm: Use Union-Find (DSU) to connect 1000 closest pairs
 * 
 * Usage:
 *   node Day8.1.js                    # Run solver only
 *   node Day8.1.js --visualize        # Run solver and generate 3D visualization
 */

import { readLines, logger, getCallerDir } from '../../lib/index.js';
import * as fs from 'fs';
import { resolve, dirname } from 'path';

const title = 'Day 8, Part 1';

logger.setDebug(false);

/**
 * Parse a single line of input - coordinates in X,Y,Z format
 * @param {string} line - Raw input line (e.g., "162,817,812")
 * @returns {object} Object with x, y, z coordinates
 */
function decodeLine(line) {
    const [x, y, z] = line.split(',').map(Number);
    return { x, y, z };
}

/**
 * Calculate Euclidean distance between two 3D points
 * @param {object} p1 - Point 1 with x, y, z
 * @param {object} p2 - Point 2 with x, y, z
 * @returns {number} Distance between points
 */
function distance(p1, p2) {
    const dx = p1.x - p2.x;
    const dy = p1.y - p2.y;
    const dz = p1.z - p2.z;
    return Math.sqrt(dx * dx + dy * dy + dz * dz);
}

/**
 * Union-Find (Disjoint Set Union) data structure
 */
class UnionFind {
    constructor(n) {
        this.parent = Array.from({ length: n }, (_, i) => i);
        this.rank = Array(n).fill(0);
        this.size = Array(n).fill(1);
    }

    find(x) {
        if (this.parent[x] !== x) {
            this.parent[x] = this.find(this.parent[x]); // Path compression
        }
        return this.parent[x];
    }

    union(x, y) {
        const rootX = this.find(x);
        const rootY = this.find(y);

        if (rootX === rootY) return false; // Already in same set

        // Union by rank
        if (this.rank[rootX] < this.rank[rootY]) {
            this.parent[rootX] = rootY;
            this.size[rootY] += this.size[rootX];
        } else if (this.rank[rootX] > this.rank[rootY]) {
            this.parent[rootY] = rootX;
            this.size[rootX] += this.size[rootY];
        } else {
            this.parent[rootY] = rootX;
            this.size[rootX] += this.size[rootY];
            this.rank[rootX]++;
        }

        return true;
    }

    getComponentSize(x) {
        return this.size[this.find(x)];
    }

    getAllComponentSizes() {
        const sizeMap = {};
        for (let i = 0; i < this.parent.length; i++) {
            const root = this.find(i);
            if (!sizeMap[root]) {
                sizeMap[root] = this.size[root];
            }
        }
        return Object.values(sizeMap).sort((a, b) => b - a);
    }
}

function solve(enableVisualization = false) {
    const scriptDir = getCallerDir(import.meta.url);
    const inputPath = resolve(scriptDir, '..', 'Input', 'Day8.txt');
    
    logger.info(`Reading input from: ${inputPath}`);
    
    // Read all lines
    const lines = readLines(inputPath, { skipEmpty: true });
    logger.info(`Total junctions: ${lines.length}`);
    
    // Parse all junction positions
    const junctions = lines.map((line, index) => ({
        id: index,
        ...decodeLine(line)
    }));
    
    // Calculate all pairwise distances
    logger.info('Calculating distances between all pairs...');
    const pairs = [];
    
    for (let i = 0; i < junctions.length; i++) {
        for (let j = i + 1; j < junctions.length; j++) {
            const dist = distance(junctions[i], junctions[j]);
            pairs.push({
                i,
                j,
                distance: dist
            });
        }
    }
    
    // Sort by distance (ascending)
    logger.info(`Sorting ${pairs.length} pairs by distance...`);
    pairs.sort((a, b) => a.distance - b.distance);
    
    // Initialize Union-Find structure
    const uf = new UnionFind(junctions.length);
    const connections = [];
    
    // For sample data (20 junctions) connect 10 pairs
    // For real data (1000+ junctions) connect 1000 pairs
    const connectLimit = lines.length <= 20 ? 10 : 1000;
    logger.info(`Connecting closest ${connectLimit} pairs...`);
    
    for (let i = 0; i < connectLimit; i++) {
        const { i: idI, j: idJ } = pairs[i];
        uf.union(idI, idJ);
        connections.push({ i: idI, j: idJ });
    }
    
    // Get all circuit sizes
    const circuitSizes = uf.getAllComponentSizes();
    logger.info(`Total circuits: ${circuitSizes.length}`);
    logger.info(`Circuit sizes: ${circuitSizes.join(', ')}`);
    
    // Get the three largest circuits
    const topThree = circuitSizes.slice(0, 3);
    logger.info(`Three largest: ${topThree.join(', ')}`);
    
    // Multiply together the sizes of the three largest circuits
    const result = topThree.reduce((acc, size) => acc * size, 1);
    
    // Generate visualization if requested
    if (enableVisualization) {
        logger.info('Generating 3D visualization...');
        const outputDir = resolve(dirname(inputPath), '..', 'Output');
        if (!fs.existsSync(outputDir)) {
            fs.mkdirSync(outputDir, { recursive: true });
        }
        generateVisualization(junctions, connections, outputDir);
    }
    
    return result;
}

function generateVisualization(junctions, connections, outputDir) {
    // Normalize coordinates for better visualization
    let minX = junctions[0].x, maxX = junctions[0].x;
    let minY = junctions[0].y, maxY = junctions[0].y;
    let minZ = junctions[0].z, maxZ = junctions[0].z;

    for (const j of junctions) {
        minX = Math.min(minX, j.x);
        maxX = Math.max(maxX, j.x);
        minY = Math.min(minY, j.y);
        maxY = Math.max(maxY, j.y);
        minZ = Math.min(minZ, j.z);
        maxZ = Math.max(maxZ, j.z);
    }

    const rangeX = maxX - minX || 1;
    const rangeY = maxY - minY || 1;
    const rangeZ = maxZ - minZ || 1;

    // Normalize junctions to range [0, 100]
    const normalizedJunctions = junctions.map(j => ({
        ...j,
        nx: ((j.x - minX) / rangeX) * 100,
        ny: ((j.y - minY) / rangeY) * 100,
        nz: ((j.z - minZ) / rangeZ) * 100
    }));

    // Generate Three.js scene
    const junctionData = normalizedJunctions.map((j, idx) => 
        `{ pos: [${j.nx.toFixed(2)}, ${j.ny.toFixed(2)}, ${j.nz.toFixed(2)}], id: ${j.id}, label: 'J${j.id}' }`
    ).join(',\n            ');

    const connectionData = connections.map(c =>
        `[${c.i}, ${c.j}]`
    ).join(',\n            ');

    const htmlContent = `<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Advent of Code 2025 - Day 8 Visualization</title>
    <style>
        body {
            margin: 0;
            overflow: hidden;
            font-family: 'Courier New', monospace;
            background: #000;
        }
        #canvas {
            display: block;
            width: 100%;
            height: 100vh;
        }
        #info {
            position: absolute;
            top: 10px;
            left: 10px;
            color: #0f0;
            background: rgba(0, 0, 0, 0.8);
            padding: 15px;
            border-radius: 5px;
            border: 1px solid #0f0;
            font-size: 12px;
            z-index: 10;
        }
        #controls {
            position: absolute;
            bottom: 10px;
            left: 10px;
            color: #0f0;
            background: rgba(0, 0, 0, 0.8);
            padding: 15px;
            border-radius: 5px;
            border: 1px solid #0f0;
            font-size: 12px;
            z-index: 10;
        }
        .stat-label {
            color: #0f0;
            font-weight: bold;
        }
        .stat-value {
            color: #0ff;
        }
    </style>
</head>
<body>
    <div id="info">
        <div><span class="stat-label">Junctions:</span> <span class="stat-value">${junctions.length}</span></div>
        <div><span class="stat-label">Connections:</span> <span class="stat-value">${connections.length}</span></div>
        <div style="margin-top: 10px; font-size: 11px;">
            <div><span class="stat-label">X Range:</span> ${minX.toFixed(0)} - ${maxX.toFixed(0)}</div>
            <div><span class="stat-label">Y Range:</span> ${minY.toFixed(0)} - ${maxY.toFixed(0)}</div>
            <div><span class="stat-label">Z Range:</span> ${minZ.toFixed(0)} - ${maxZ.toFixed(0)}</div>
        </div>
    </div>
    <div id="controls">
        <div><strong>Controls:</strong></div>
        <div>- Drag to rotate</div>
        <div>- Scroll to zoom</div>
        <div>- Right-click drag to pan</div>
    </div>

    <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r128/three.min.js"><\/script>
    <script>
        // Setup scene
        const scene = new THREE.Scene();
        scene.background = new THREE.Color(0x000000);
        scene.fog = new THREE.Fog(0x000000, 500, 800);

        const canvas = document.createElement('canvas');
        document.body.appendChild(canvas);
        
        const renderer = new THREE.WebGLRenderer({ canvas, antialias: true });
        renderer.setSize(window.innerWidth, window.innerHeight);
        renderer.setPixelRatio(window.devicePixelRatio);

        const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
        camera.position.set(60, 60, 60);
        camera.lookAt(50, 50, 50);

        // Lighting
        const ambientLight = new THREE.AmbientLight(0xffffff, 0.5);
        scene.add(ambientLight);

        const directionalLight = new THREE.DirectionalLight(0xffffff, 0.8);
        directionalLight.position.set(100, 100, 100);
        scene.add(directionalLight);

        // Create junction points
        const junctions = [
            ${junctionData}
        ];

        const geometry = new THREE.BufferGeometry();
        const positions = [];
        const junctionMap = {};

        junctions.forEach((j, idx) => {
            positions.push(j.pos[0], j.pos[1], j.pos[2]);
            junctionMap[idx] = j;
        });

        geometry.setAttribute('position', new THREE.BufferAttribute(new Float32Array(positions), 3));

        const pointsMaterial = new THREE.PointsMaterial({
            color: 0x00ff00,
            size: 0.8,
            sizeAttenuation: true
        });

        const points = new THREE.Points(geometry, pointsMaterial);
        scene.add(points);

        // Create connection lines
        const connections = [
            ${connectionData}
        ];

        const lineGeometry = new THREE.BufferGeometry();
        const linePositions = [];

        connections.forEach(([i, j]) => {
            const p1 = junctions[i].pos;
            const p2 = junctions[j].pos;
            linePositions.push(p1[0], p1[1], p1[2]);
            linePositions.push(p2[0], p2[1], p2[2]);
        });

        lineGeometry.setAttribute('position', new THREE.BufferAttribute(new Float32Array(linePositions), 3));
        
        const lineMaterial = new THREE.LineBasicMaterial({
            color: 0x0088ff,
            opacity: 0.8,
            transparent: true,
            linewidth: 2
        });

        const lines = new THREE.LineSegments(lineGeometry, lineMaterial);
        scene.add(lines);

        // Add grid for reference
        const gridHelper = new THREE.GridHelper(120, 12, 0x444444, 0x222222);
        gridHelper.position.y = 0;
        scene.add(gridHelper);

        // Add axes helper
        const axesHelper = new THREE.AxesHelper(30);
        scene.add(axesHelper);

        // Mouse controls
        let isDragging = false;
        let previousMousePosition = { x: 0, y: 0 };

        document.addEventListener('mousedown', (e) => {
            isDragging = true;
            previousMousePosition = { x: e.clientX, y: e.clientY };
        });

        document.addEventListener('mousemove', (e) => {
            if (isDragging) {
                const deltaX = e.clientX - previousMousePosition.x;
                const deltaY = e.clientY - previousMousePosition.y;

                if (e.button === 0) {
                    // Left button - rotate
                    camera.position.applyAxisAngle(new THREE.Vector3(0, 1, 0), deltaX * 0.01);
                    camera.position.applyAxisAngle(
                        new THREE.Vector3(1, 0, 0).applyAxisAngle(new THREE.Vector3(0, 1, 0), -deltaX * 0.01),
                        deltaY * 0.01
                    );
                }
                previousMousePosition = { x: e.clientX, y: e.clientY };
            }
        });

        document.addEventListener('mouseup', () => {
            isDragging = false;
        });

        document.addEventListener('wheel', (e) => {
            e.preventDefault();
            const zoomSpeed = 5;
            const direction = camera.position.clone().normalize();
            const distance = camera.position.length();
            const newDistance = e.deltaY > 0 ? distance + zoomSpeed : distance - zoomSpeed;
            camera.position.copy(direction.multiplyScalar(Math.max(10, newDistance)));
        }, { passive: false });

        // Window resize handler
        window.addEventListener('resize', () => {
            camera.aspect = window.innerWidth / window.innerHeight;
            camera.updateProjectionMatrix();
            renderer.setSize(window.innerWidth, window.innerHeight);
        });

        // Animation loop
        function animate() {
            requestAnimationFrame(animate);
            renderer.render(scene, camera);
        }

        animate();
    <\/script>
</body>
</html>`;

    const htmlPath = resolve(outputDir, 'visualization.html');
    fs.writeFileSync(htmlPath, htmlContent);
    logger.info(`✓ Visualization saved to: ${htmlPath}`);
}

// Parse command line arguments
const args = process.argv.slice(2);
let enableVisualization = false;

if (args.includes('--visualize')) {
    enableVisualization = true;
}

try {
    logger.section(title);
    const startTime = performance.now();
    const result = solve(enableVisualization);
    const endTime = performance.now();
    
    logger.result('Final Answer', result);
    logger.success(`Completed in ${(endTime - startTime).toFixed(2)}ms`);
} catch (error) {
    logger.error('Failed to solve:', error.message);
    console.error(error);
    process.exit(1);
}
