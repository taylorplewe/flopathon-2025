<script setup>
import  VolumeDisplay  from "@/components/screaming-fingers/VolumeDisplay.vue";
import ChangeVolumeButton  from "@/components/screaming-fingers/ChangeVolumeButton.vue";
import TypingTest  from "@/components/screaming-fingers/TypingTest.vue";
import VolumeConfirmation  from "@/components/screaming-fingers/VolumeConfirmation.vue";
</script>

<template>
	  <div class="screaming-fingers">
		<h1>Screaming Fingers</h1>
		<VolumeDisplay />
		<ChangeVolumeButton />
		<TypingTest />
		<VolumeConfirmation />
	  </div>
</template>

<style scoped>
</style>
