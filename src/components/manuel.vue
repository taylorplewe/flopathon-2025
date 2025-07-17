<template>
    <div>
        <button class="modal-btn" @click="showModal = true">Open Modal</button>

        <Modal :is-open="showModal" @close="showModal = false">
            <template #header>
                <h3>Choose your volume</h3>
            </template>
            <template #body>
                <div class="maze-wrapper">
                    <h3>Current Volume: {{ volume }}</h3>
                    <maze-generator @new-volume="volume = $event" @hit-wall="onHitWall" @hit-end="volume++" />
                </div>
            </template>
            <template #footer>
                <button class="btn-green" @click="showModal = false">Got It!</button>
            </template>
        </Modal>
        <div v-if="showModal" id="overlay" ref="overlayRef"></div>
    </div>
</template>

<script setup>
import { ref, onMounted, onUnmounted } from 'vue';
import Modal from './maze/Modal.vue';
import MazeGenerator from './maze/MazeGenerator.vue';

const showModal = ref(false);

const onHitWall = () => {
    showModal.value = false;
    volume.value = 0; // Reset volume when hitting a wall
}

const volume = ref(0);

const overlayRef = ref(null);

// Listen for mousemove events on the entire document
onMounted(() => {
    const handleMouseMove = (e) => {
        const mouseX = e.clientX;
        const mouseY = e.clientY;
        if (overlayRef.value) {
            overlayRef.value.style.setProperty('--mouse-x', `${mouseX}px`);
            overlayRef.value.style.setProperty('--mouse-y', `${mouseY}px`);
        }
    };
    document.addEventListener('mousemove', handleMouseMove);
    onUnmounted(() => {
        document.removeEventListener('mousemove', handleMouseMove);
    });
});
</script>

<style>
body {
    font-family: sans-serif;
    margin: 20px;
    background-color: #f0f2f5;
    display: flex;
    justify-content: center;
    align-items: center;
    min-height: 100vh;
}

h1 {
    color: #333;
}

.modal-btn {
    position: absolute;
    right: 20px;
    top: 200px;
}

button {
    padding: 10px 20px;
    font-size: 16px;
    cursor: pointer;
    border: none;
    border-radius: 5px;
    background-color: #007bff;
    color: white;
    transition: background-color 0.3s ease;
}

button:hover {
    background-color: #0056b3;
}

.maze-wrapper {
    width: 100%;
    height: 100%;
    display: flex;
    flex-direction: column;
    justify-content: center;
    align-items: center;
}

#overlay {
    position: fixed;
    /* Fixed position relative to the viewport */
    top: 0;
    left: 0;
    width: 100vw;
    /* Full viewport width */
    height: 100vh;
    /* Full viewport height */
    background: radial-gradient(circle at var(--mouse-x) var(--mouse-y), transparent 0px, transparent 80px, rgba(0, 0, 0, 0.9) 120px);
    /* Initial gradient: transparent in the center, black outside */
    pointer-events: none;
    /* Allows clicks/interactions to pass through the overlay */
    transition: background 0.05s linear;
    /* Smooth transition for the flashlight movement */
    border-radius: 0;
    /* No rounded corners for the overlay itself */
    z-index: 20;
    /* Ensure the overlay is on top of the content */
}
</style>