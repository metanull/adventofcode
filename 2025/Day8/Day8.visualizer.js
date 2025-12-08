/**
 * Advent of Code 2025 - Day 8, Part 1 - 3D Visualizer
 * 
 * This module provides 3D visualization of junction boxes and connections
 * It is separate from the solver and can be enabled/disabled independently
 * 
 * Usage:
 *   import { createVisualizer } from './Day8.visualizer.js';
 *   const visualizer = createVisualizer(junctions, connections, outputPath);
 *   visualizer.render();
 */

import * as fs from 'fs';
import * as path from 'path';

/**
 * Creates a 3D visualizer for the junction boxes problem
 * @param {array} junctions - Array of junction objects with x, y, z coordinates and id
 * @param {array} connections - Array of connection objects with i, j indices
 * @param {string} outputDir - Directory to save the visualization output
 * @returns {object} Visualizer object with render methods
 */
export function createVisualizer(junctions, connections, outputDir = './output') {
    // Ensure output directory exists
    if (!fs.existsSync(outputDir)) {
        fs.mkdirSync(outputDir, { recursive: true });
    }

    /**
     * Generate an HTML/Three.js based 3D visualization
     */
    function generateHTMLVisualization() {
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

    <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/r128/three.min.js"></script>
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
            size: 2,
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
            opacity: 0.6,
            transparent: true,
            linewidth: 1
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
    </script>
</body>
</html>`;

        return htmlContent;
    }

    /**
     * Generate a JSON representation of the visualization data
     */
    function generateJSONVisualization() {
        return JSON.stringify({
            metadata: {
                generated: new Date().toISOString(),
                junctionCount: junctions.length,
                connectionCount: connections.length
            },
            junctions: junctions,
            connections: connections
        }, null, 2);
    }

    return {
        /**
         * Render the visualization and save to files
         */
        render: function(format = 'all') {
            console.log('🎨 Generating 3D visualization...');

            if (format === 'html' || format === 'all') {
                const htmlPath = path.join(outputDir, 'visualization.html');
                const html = generateHTMLVisualization();
                fs.writeFileSync(htmlPath, html);
                console.log(`✓ HTML visualization saved to: ${htmlPath}`);
            }

            if (format === 'json' || format === 'all') {
                const jsonPath = path.join(outputDir, 'visualization.json');
                const json = generateJSONVisualization();
                fs.writeFileSync(jsonPath, json);
                console.log(`✓ JSON data saved to: ${jsonPath}`);
            }

            return {
                htmlPath: path.join(outputDir, 'visualization.html'),
                jsonPath: path.join(outputDir, 'visualization.json')
            };
        },

        /**
         * Get statistics about the visualization
         */
        getStats: function() {
            return {
                junctionCount: junctions.length,
                connectionCount: connections.length,
                boundingBox: {
                    minX: Math.min(...junctions.map(j => j.x)),
                    maxX: Math.max(...junctions.map(j => j.x)),
                    minY: Math.min(...junctions.map(j => j.y)),
                    maxY: Math.max(...junctions.map(j => j.y)),
                    minZ: Math.min(...junctions.map(j => j.z)),
                    maxZ: Math.max(...junctions.map(j => j.z))
                }
            };
        }
    };
}
