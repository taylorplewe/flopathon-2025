<template>
    <header>(the volume is determined by how similar your painting is to the painting on the left!)</header>
    <main>
        <div class="canvases">
            <div class="canvas-with-label">
                <label for="target">Target version</label>
                <canvas
                    id="target"
                    ref="target"
                    width="128"
                    height="128"
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
                    @contextmenu="e => e.preventDefault()"
                ></canvas>
            </div>
        </div>
    </main>
</template>

<script setup lang="ts">
import { onMounted, onUnmounted, useTemplateRef } from 'vue';

import Draw from './a.out.js';

const targetRef = useTemplateRef('target');
const drawableRef = useTemplateRef('drawable');

onMounted(() => {
    document.documentElement.style.overflow = 'hidden';

    const img = new Image();
    console.log(img);
    img.addEventListener('load', e => console.log('loaded!', e))
    img.onload = () => {
        const targetCtx = targetRef.value.getContext('2d');
        targetCtx.drawImage(img, 0, 0);
        console.log(targetCtx);
    }
    img.src = 'src/components/einstein.bmp';

    (async () => {
        const mod = await Draw({
            canvas: drawableRef.value,
        });
    })();
});

onUnmounted(() => {
    document.documentElement.style.overflow = 'revert';
})
</script>

<style scoped>
header {
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
}
canvas {
    padding: 8px;
    border-radius: 8px;
    border: 1px solid #ccc;

    width: calc(128px * 3);
    image-rendering: pixelated;
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
</style>