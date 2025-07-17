<script setup>
import { ref } from 'vue';
import StartButton from '../components/screaming-fingers/StartButton.vue';
import TypingTest from '../components/screaming-fingers/TypingTest.vue';
import ConfirmationDialog from '../components/screaming-fingers/ConfirmationDialog.vue';

const showTyping = ref(false);
const showConfirmation = ref(false);
const finalVolume = ref(0);

const onTypingComplete = (successPercentage) => {
  finalVolume.value = successPercentage;
  showTyping.value = false;
  showConfirmation.value = true;
}

const resetGame = () => {
  showTyping.value = false;
  showConfirmation.value = false;
  finalVolume.value = 0;
}

const saveVolume = () => {
  console.log('Final Volume Set To:', finalVolume.value);
}
</script>

<template>
  <div class="main-page">
    <StartButton v-if="!showTyping" @start="showTyping = true" />
    <TypingTest v-if="showTyping" @complete="onTypingComplete" />
    <ConfirmationDialog v-if="showConfirmation" :volume="finalVolume" @success="saveVolume" @fail="resetGame" />
  </div>
</template>

