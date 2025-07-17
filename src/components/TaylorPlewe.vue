<template>
    <div id="container">
        <div id="crank">
        </div>

        <div id="slider">
            <div id="fill"></div>
        </div>
    </div>
</template>

<script setup>
</script>

<style scoped>
#container {
    position: fixed;
    top: 50%;
    left: 50%;
    translate: -50% -50%;
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
