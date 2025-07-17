<script setup>
import { ref } from 'vue';
import StartButton from '../components/screaming-fingers/StartButton.vue';
import TypingTest from '../components/screaming-fingers/TypingTest.vue';
import ConfirmationDialog from '../components/screaming-fingers/ConfirmationDialog.vue';

const typingAudio = [
  '../../../public/sounds/typing/big-hit.mp3',
  '../../../public/sounds/typing/censor.mp3',
  '../../../public/sounds/typing/click.mp3',
  '../../../public/sounds/typing/creaking.mp3',
  '../../../public/sounds/typing/glass.mp3',
  '../../../public/sounds/typing/hit.mp3',
  '../../../public/sounds/typing/horn.mp3',
  '../../../public/sounds/typing/needle.mp3',
  '../../../public/sounds/typing/pipe.mp3',
  '../../../public/sounds/typing/pop.mp3',
  '../../../public/sounds/typing/rise.mp3',
  '../../../public/sounds/typing/snap.mp3',
  '../../../public/sounds/typing/surprise.mp3',
  '../../../public/sounds/typing/thriller.mp3',
  '../../../public/sounds/typing/violin.mp3',
  '../../../public/sounds/typing/whistle.mp3',
]

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
    <TypingTest v-if="showTyping" :typing-audio="typingAudio" @complete="onTypingComplete" />
    <ConfirmationDialog v-if="showConfirmation" :volume="finalVolume" @success="saveVolume" @fail="resetGame" />
  </div>
</template>

