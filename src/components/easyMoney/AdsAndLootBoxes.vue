<script setup lang="ts">
import WatchAd from './WatchAd.vue'
import LootBox from './LootBox.vue'
import { useVolumeStore } from './useVolumeStore';
import { storeToRefs } from 'pinia'

const {isViewingAd} = storeToRefs(useVolumeStore())
</script>

<template>
  <component :is="isViewingAd ? WatchAd : LootBox"></component>
</template>

<style scoped>

</style>