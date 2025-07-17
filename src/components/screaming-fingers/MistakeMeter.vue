<script setup>
import { computed } from 'vue'

const props = defineProps({
  mistakes: Number,
  keystrokes: Number,
})

const mistakeRate = computed(() => {
  return props.keystrokes === 0 ? 0 : props.mistakes / props.keystrokes
})

const meterHeight = computed(() => `${mistakeRate.value * 100}%`)
</script>

<template>
  <div class="mistake-meter">
    <div class="fill" :style="{ height: meterHeight }"></div>
  </div>
</template>

<style scoped>
.mistake-meter {
  height: 200px;
  width: 20px;
  background: linear-gradient(to top, red, yellow, green);
  border: 2px solid black;
}
.fill {
  width: 100%;
  background-color: rgba(0,0,0,0.2);
  transition: height 0.2s;
}
</style>
