<script setup>
import { ref, reactive, computed, onMounted } from 'vue'
import MistakeMeter from './MistakeMeter.vue'
import VolumeDisplay from './VolumeSlider.vue'

const typingAudio = [
  new Audio('../../../public/sounds/typing/big-hit.mp3'),
  new Audio('../../../public/sounds/typing/censor.mp3'),
  new Audio('../../../public/sounds/typing/click.mp3'),
  new Audio('../../../public/sounds/typing/creaking.mp3'),
  new Audio('../../../public/sounds/typing/glass.mp3'),
  new Audio('../../../public/sounds/typing/hit.mp3'),
  new Audio('../../../public/sounds/typing/horn.mp3'),
  new Audio('../../../public/sounds/typing/needle.mp3'),
  new Audio('../../../public/sounds/typing/pipe.mp3'),
  new Audio('../../../public/sounds/typing/pop.mp3'),
  new Audio('../../../public/sounds/typing/rise.mp3'),
  new Audio('../../../public/sounds/typing/snap.mp3'),
  new Audio('../../../public/sounds/typing/surprise.mp3'),
  new Audio('../../../public/sounds/typing/thriller.mp3'),
  new Audio('../../../public/sounds/typing/violin.mp3'),
  new Audio('../../../public/sounds/typing/whistle.mp3'),
]
const words = ref([])
const input = ref('')
const currentWordIndex = ref(0)
const mistakes = ref(0)
const keystrokes = ref(0)
const startTime = ref(null)
const backgroundColor = ref('#fff')

const WPM = computed(() => {
  if (!startTime.value) return 0
  const minutes = (Date.now() - startTime.value) / 60000
  return Math.floor((keystrokes.value / 5) / minutes)
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
  if (e.key === 'Backspace') {
    // Chaos mode: don't allow it!
    e.preventDefault()
    return
  }

  const expectedChar = words.value.join(' ')[input.value.length] || ''
  if (e.key.length === 1) {
    keystrokes.value++
    input.value += e.key

    if (e.key !== expectedChar) {
      mistakes.value++
      playAnnoyingSound()
      flashBackground()
    } else {
      playTypingSound()
    }
  }
}

function flashBackground() {
  const color = `hsl(${Math.floor(Math.random() * 360)}, 100%, 50%)`
  backgroundColor.value = color
}

function playAnnoyingSound() {
	const randomIndex = Math.floor(Math.random() * typingAudio.length)
	const sound = typingAudio[randomIndex]
	sound.volume = 1
	sound.play()
}

function playTypingSound() {
  const click = new Audio('../../../public/sounds/typing/glass.mp3')
  click.volume = Math.min(1, WPM.value / 100)
  click.play()
}

onMounted(() => {
  generateWords()
  startTime.value = Date.now()
  window.addEventListener('keydown', handleKeydown)
})
</script>

<template>
  <div :style="{ backgroundColor }" class="typing-test">
    <div class="words">{{ words.join(' ') }}</div>
    <input v-model="input" disabled class="invisible-input" />
    <MistakeMeter :mistakes="mistakes" :keystrokes="keystrokes" />
    <VolumeDisplay :volume="successPercentage" />
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
