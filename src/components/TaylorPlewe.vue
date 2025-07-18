<template>
    <p id="header">(the volume is determined by how similar your painting is to the painting on the left)</p>
    <main>
        <div class="canvases">
            <div class="canvas-with-label">
                <label for="target">Target version</label>
                <canvas
                    id="target"
                    ref="target"
                    width="128"
                    height="128"
                    tabindex="-1"
                    @contextmenu="e => e.preventDefault()"
                ></canvas>
            </div>
        
            <div class="canvas-with-label">
                <label for="drawable">Your version</label>
                <canvas
                    id="drawable"
                    ref="drawable"
                    width="128"
                    height="128"
                    tabindex="-1"
                    @contextmenu="e => e.preventDefault()"
                ></canvas>
            </div>
        </div>

        <div id="slider">
            <div id="fill" :style="`width: ${volume}%`"></div>
        </div>
    </main>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted, useTemplateRef } from 'vue';

import Draw from './a.out.js';

const targetRef = useTemplateRef('target');
const drawableRef = useTemplateRef('drawable');

const volume = ref<number>(0);

let getMatchPercentage: Function;
let animationRequestHandle: number;
const updateVolumeFromMatchPercentage = () => {
    volume.value = getMatchPercentage() * 100;
    animationRequestHandle = requestAnimationFrame(updateVolumeFromMatchPercentage);
}

onMounted(() => {
    document.documentElement.style.overflow = 'hidden';

    // draw image on left canvas
    const img = new Image();
    img.addEventListener('load', e => console.log('loaded!', e))
    img.onload = () => {
        const targetCtx = targetRef.value.getContext('2d');
        targetCtx.drawImage(img, 0, 0);
    }
    img.src = 'src/components/einstein.bmp';

    // load WebAssembly module
    (async () => {
        const mod = await Draw({
            canvas: drawableRef.value,
        });
        getMatchPercentage = mod._get_match_percentage;
        animationRequestHandle = requestAnimationFrame(updateVolumeFromMatchPercentage);
    })();
});

onUnmounted(() => {
    document.documentElement.style.overflow = 'revert';
    cancelAnimationFrame(animationRequestHandle);
})
</script>

<style scoped>
p#header {
    color: #777;
    font-family: sans-serif;
    position: fixed;
    top: 56px;
    left: 50%;
    translate: -50% 0;
}
main {
    position: fixed;
    top: 50%;
    left: 50%;
    translate: -50% -50%;

    display: flex;
    flex-direction: column;
    gap: 32px;
    align-items: center;
}
canvas {
    padding: 8px;
    border-radius: 8px;
    border: 1px solid #ccc;

    width: calc(128px * 3);
    image-rendering: pixelated;

    &#drawable {
        cursor: none !important; /* fighting with emscripten's generated draw.js */
    }
}

.canvases {
    display: flex;
    align-items: center;
    gap: 64px;
}
.canvas-with-label {
    color: #777;
    font-family: sans-serif;
    display: flex;
    flex-direction: column;
    align-items: center;    
    gap: 8px;
}
#slider {
    width: 400px;
    height: 16px;
    background-color: #eee;
    border-radius: 4px;
    position: relative;
    overflow: hidden;

    #fill {
        position: absolute;
        height: 100%;
        background-color: #4b4;
    }
}
</style>