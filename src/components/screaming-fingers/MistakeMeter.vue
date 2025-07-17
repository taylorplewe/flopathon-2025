<script setup>
import { computed } from 'vue'

const props = defineProps({
	mistakes: Number,
	keystrokes: Number,
})

const meterHeight = computed(() => `${props.mistakes}%`)
</script>

<template>
	<div class="mistake-meter">
		<div class="meter-fill" :style="{ height: meterHeight }"></div>
	</div>
</template>

<style scoped>
.mistake-meter {
	height: 200px;
	width: 20px;
	border: 2px solid black;
	display: flex;
	flex-direction: column;
	justify-content: flex-end; /* Align fill at the bottom */
}
.meter-fill {
	width: 100%;
	background: linear-gradient(to top, transparent,green, yellow, red); /* Gradient flows upward */
}
</style>