<template>
    <div id="container">
        <div id="crank">
            <div id="center">
                <img src="../assets/brass-circle.png" alt="center" />
            </div>
            <div id="bar" ref="bar" :style="`width: ${BAR_LENGTH + 12}px;`">
                <!-- <img src="../assets/crank-arm.png" alt="crank arm" /> -->
            </div>
            <!-- <div id="handle" ref="handle" style="left: 0; top: 0;" @mousedown="grabHandle"></div> -->
            <div id="handle" ref="handle" style="left: 0; top: 0;" @mousedown="grabHandle">
                <img src="../assets/handle.png" alt="handle" /> 
            </div>
        </div>

        <div id="slider">
            <div id="fill" :style="`width: ${val}%`"></div>
        </div>
    </div>
</template>

<script setup lang="ts">
import { ref, useTemplateRef, onMounted, onUnmounted } from 'vue';

onMounted(() => document.body.style.userSelect = 'none');
onUnmounted(() => {
    document.body.style.userSelect = 'revert';
    document.documentElement.style.cursor = 'revert';
});

const BAR_LENGTH = ref(100);

const xOffsetOnGrab = ref(0);
const yOffsetOnGrab = ref(0);
const handleRef = useTemplateRef('handle');
const barRef =useTemplateRef('bar');
const isGrabbingHandle = ref(false);

const val = ref(0);

window.addEventListener('mouseup', () => {
    if (isGrabbingHandle.value) {
        isGrabbingHandle.value = false;
        handleRef.value.style.cursor = 'grab';
        document.documentElement.style.cursor = 'revert';
    }
});

window.addEventListener('mousemove', e => {
    if (!isGrabbingHandle.value) return;

    const desiredX = e.clientX + xOffsetOnGrab.value;
    const desiredY = e.clientY + yOffsetOnGrab.value;

    const hyp = Math.sqrt(desiredX * desiredX + desiredY * desiredY);
    const factor = BAR_LENGTH.value / hyp;

    const endX = desiredX * factor;
    const endY = desiredY * factor;

    handleRef.value.style.left = `${endX}px`;
    handleRef.value.style.top = `${endY}px`;

    const deg = Math.floor((Math.atan2(desiredY, desiredX) / Math.PI) * 180);
    barRef.value.style.rotate = `${deg}deg`;
});

const grabHandle = (e: MouseEvent) => {
    console.log(e);
    isGrabbingHandle.value = true;
    handleRef.value.style.cursor = 'grabbing';
    document.documentElement.style.cursor = 'grabbing';
    xOffsetOnGrab.value = parseInt(handleRef.value.style.left) - e.clientX;
    yOffsetOnGrab.value = parseInt(handleRef.value.style.top) - e.clientY;
}
</script>

<style scoped>
#container {
    position: fixed;
    top: 50%;
    left: 50%;
    translate: -50% -50%;
}

#crank {
    position: absolute;
    left: 0;
    translate: -800% 0;

    & > #center {
        width: 32px;
        aspect-ratio: 1;
        border-radius: 100%;
        background-color: gray;
        position: relative;
        translate: -50% -50%;
        & > img {
            width: 100%;
            aspect-ratio: 1;
        }
    }

    & > #bar {
        height: 12px;
        border-radius: 6px;
        position: absolute;
        top: 0;
        left: 0;
        transform-origin: left;
        translate: 0 -50%;
        background-color: black;
        & > img {
            width: 100%;
        }
    }

    img {
        pointer-events: none;
    }

    & > #handle {
        cursor: grab;
        position: absolute;
        /*width: 64px;
        background-color: gray;
        translate: -50% -50%;
        height: 64px;*/
        translate: -16px -50%;

        & > img {
            width: 128px;
        }

        &.grabbing {
            cursor: grabbing;
        }
    }
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
