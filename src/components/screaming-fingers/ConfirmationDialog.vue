<!-- components/ConfirmationDialog.vue -->
<script setup>
import { defineProps, defineEmits } from 'vue'

const props = defineProps({
  volume: {
    type: Number,
    required: true
  }
})

const emit = defineEmits(['confirm'])

function handleConfirm() {
  emit('confirm')
}
</script>

<template>
  <div class="dialog-overlay">
    <div class="dialog-box">
      <h2>🔊 Volume Set!</h2>
      <p>Your final volume level is: <strong>{{ volume }}%</strong></p>
      <button @click="handleConfirm" class="confirm-button">Confirm</button>
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
}

.confirm-button {
  margin-top: 1rem;
  padding: 0.75rem 1.5rem;
  background-color: limegreen;
  color: white;
  border: none;
  border-radius: 8px;
  font-size: 1rem;
  cursor: pointer;
  transition: background 0.2s ease;
}

.confirm-button:hover {
  background-color: green;
}
</style>
