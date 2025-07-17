<script setup>
import { ref, onMounted, onUnmounted } from 'vue'

const props = defineProps({
  volume: {
    type: Number,
    required: true
  }
})

const emit = defineEmits(['confirm'])

const movingSlider = ref(0)
const isSliding = ref(true)
const resultMessage = ref('')
let animationInterval = null

function startSliderAnimation() {
  animationInterval = setInterval(() => {
    // Speed boost: jump 5-15 per tick
    const speed = Math.floor(Math.random() * 5) + 2
    movingSlider.value = (movingSlider.value + speed) % 101
  }, 15) // 15ms interval — max chaos
}

function stopSliderAnimation() {
  isSliding.value = false
  clearInterval(animationInterval)

  const distance = Math.abs(movingSlider.value - props.volume)
  if (distance <= 5) {
    resultMessage.value = '🎯 SUCCESS! You matched the volume.'
  } else {
    resultMessage.value = '💥 TOO BAD! That was way off.'
  }
}

function confirmVolume() {
  emit('confirm')
}

onMounted(() => {
  startSliderAnimation()
})

onUnmounted(() => {
  clearInterval(animationInterval)
})
</script>

<template>
  <div class="dialog-overlay">
    <div class="dialog-box">
      <h2>⚡ Volume Reflex Test</h2>

      <!-- Target volume slider -->
      <p>Target Volume: <strong>{{ volume }}%</strong></p>
      <input type="range" min="0" max="100" :value="volume" disabled />

      <!-- Racing slider -->
      <p class="mt">Stop the chaotic slider below:</p>
      <input type="range" min="0" max="100" :value="movingSlider" disabled />

      <button @click="stopSliderAnimation" :disabled="!isSliding" class="stop-button">
        🛑 Stop the Madness
      </button>

      <p v-if="resultMessage" class="result-message">{{ resultMessage }}</p>

      <button v-if="resultMessage" @click="confirmVolume" class="confirm-button">
        ✅ Confirm
      </button>
    </div>
  </div>
</template>

<style scoped>
.dialog-overlay {
  position: fixed;
  inset: 0;
  background-color: rgba(0, 0, 0, 0.85);
  display: flex;
  justify-content: center;
  align-items: center;
  z-index: 1000;
}

.dialog-box {
  background: white;
  border-radius: 16px;
  padding: 2rem;
  text-align: center;
  box-shadow: 0 0 20px rgba(255, 0, 200, 0.7);
  width: 420px;
  font-family: monospace;
}

input[type="range"] {
  width: 100%;
  margin: 0.5rem 0 1rem;
}

.stop-button {
  padding: 0.5rem 1.25rem;
  font-size: 1rem;
  background-color: #ff3333;
  color: white;
  border: none;
  border-radius: 8px;
  cursor: pointer;
  margin-bottom: 1rem;
  animation: shake 0.1s infinite alternate;
}

.stop-button:disabled {
  background-color: gray;
  cursor: not-allowed;
  animation: none;
}

.confirm-button {
  background-color: limegreen;
  color: white;
  padding: 0.6rem 1.5rem;
  border: none;
  border-radius: 8px;
  font-size: 1rem;
  cursor: pointer;
  transition: background 0.2s ease;
}

.confirm-button:hover {
  background-color: green;
}

.result-message {
  font-weight: bold;
  margin: 1rem 0;
  font-size: 1.2rem;
  color: #8000ff;
}

.mt {
  margin-top: 1rem;
}

@keyframes shake {
  from { transform: translateX(-2px); }
  to { transform: translateX(2px); }
}
</style>
