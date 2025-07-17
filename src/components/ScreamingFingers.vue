<template>
  <div class="main-page">
    <StartButton @start="showTyping = true" v-if="!showTyping" />
    <TypingTest v-if="showTyping" @complete="onTypingComplete" />
    <ConfirmationDialog v-if="showConfirmation" :volume="finalVolume" @confirm="saveVolume" />
  </div>
</template>

<script setup>
import { ref } from 'vue'
import TypingTest from '../components/screaming-fingers/TypingTest.vue'
import StartButton from '../components/screaming-fingers/StartButton.vue'
import ConfirmationDialog from '../components/screaming-fingers/ConfirmationDialog.vue'

const showTyping = ref(false)
const showConfirmation = ref(false)
const finalVolume = ref(0)

function onTypingComplete(successPercentage) {
  finalVolume.value = successPercentage
  showTyping.value = false
  showConfirmation.value = true
}

function saveVolume() {
  console.log('Final Volume Set To:', finalVolume.value)
}
</script>
