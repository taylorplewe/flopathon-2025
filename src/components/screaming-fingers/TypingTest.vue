<script setup>
import { ref, computed, onMounted, watch } from 'vue'
import MistakeMeter from './MistakeMeter.vue'
import VolumeDisplay from './VolumeSlider.vue'

const props = defineProps({
	typingAudio: Array
})

const typingVolume = ref(0.1)
const words = ref([])
const input = ref('')
const mistakes = ref(0)
const keystrokes = ref(0)
const startTime = ref(null)
const backgroundColor = ref('#fff')
const remainingTime = ref(10)
const timerInterval = ref(null)
const loading = ref(true)
const audioLoaded = ref(false)

function checkAudioLoaded(audioSources) {
	let loadedCount = 0
	audioSources.forEach((source) => {
		const audio = new Audio(source)
		audio.preload = 'auto'

		audio.addEventListener('canplaythrough', () => {
			loadedCount++
			if (loadedCount === audioSources.length) {
				audioLoaded.value = true
				loading.value = false
			}
		})
	})
}
function startTimer() {
	remainingTime.value = 10
	timerInterval.value = setInterval(() => {
		if (remainingTime.value > 0) {
			remainingTime.value--
		} else {
			clearInterval(timerInterval.value)
		}
	}, 1000)
}

const WPM = computed(() => {
	if (!startTime.value || isNaN(keystrokes.value)) return 0
	const minutes = (Date.now() - startTime.value) / 60000
	if (!minutes || minutes <= 0) return 0
	const wpm = (keystrokes.value / 5) / minutes
	return isFinite(wpm) && wpm >= 0 ? Math.floor(wpm) : 0
})

const successPercentage = computed(() => {
	const total = keystrokes.value
	const errorFree = total - mistakes.value
	return total === 0 ? 0 : Math.max(0, Math.floor((errorFree / total) * 100))
})

function generateWords() {
	const sampleWords = ['banana', 'chaos', 'fizz', 'weird', 'nonsense', 'vortex', 'echo', 'taco', 'pogo', 'explode']
	words.value = Array.from({ length: 10 }, () => sampleWords[Math.floor(Math.random() * sampleWords.length)])
}

function handleKeydown(e) {
	console.log("Key pressed:", e.key)
	flashBackground()
	playTypingSound()
	if (e.key === 'Backspace') {
		e.preventDefault()
		return
	}

	const expectedChar = words.value.join(' ')[input.value.length] || ''
	if (e.key.length === 1) {
		keystrokes.value++
		input.value += e.key

		if (e.key !== expectedChar) {
			mistakes.value++
		}
	}
}

watch(WPM, (newWPM) => {
	if (newWPM >= 100) {
		typingVolume.value = 1
	} else {
		typingVolume.value = Math.min(1, Math.max(0.1, newWPM / 100))
	}
	console.log("WPM changed:", newWPM, "Volume set to:", typingVolume.value)
})

function flashBackground() {
	const color = `hsl(${Math.floor(Math.random() * 360)}, 100%, 50%)`
	backgroundColor.value = color
}

function playTypingSound() {
	console.log("Playing typing sound at volume:", typingVolume.value)
	const randomIndex = Math.floor(Math.random() * props.typingAudio.length)
	const sound = new Audio(props.typingAudio[randomIndex])
	console.log("sound", sound)
	sound.volume = typingVolume.value
	sound.play()
	// sound.currentTime = 0 // Reset to start for immediate playback
}
 onMounted(() => {
	 generateWords()
	 startTimer()
	 startTime.value = Date.now()
	 document.addEventListener('keydown', handleKeydown)
	 checkAudioLoaded(props.typingAudio)
 })
</script>

<template>
  <div v-if="audioLoaded" :style="{ backgroundColor }" class="typing-test">
    <div class="words">{{ words.join(' ') }}</div>
    <input v-model="input" disabled class="invisible-input" />
    <MistakeMeter :mistakes="mistakes" :keystrokes="keystrokes" />
	  <p>Mistake Meter</p>
	  <div>
		  <p>Current WPM: {{ WPM }}</p>
        <VolumeDisplay :volume="successPercentage" />
	  </div>
	  <div>
		  <p>Time Remaining: {{ remainingTime }} seconds</p>
	  </div>
  </div>
</template>

<style scoped>
.typing-test {
  padding: 2rem;
  font-family: monospace;
  font-size: 1.5rem;
}
.invisible-input {
  position: absolute;
  opacity: 0;
}
</style>
