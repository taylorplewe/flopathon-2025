<script setup>
import { RouterLink, RouterView, useRouter } from 'vue-router'

// Get all routes from the router to dynamically generate navigation
const router = useRouter()
const routes = router.options.routes
</script>

<template>
  <header>
    <nav class="main-nav">
      <!-- Dynamically render router links for all routes -->
      <RouterLink v-for="route in routes" :key="route.name" :to="route.path">
        {{ route.meta?.displayName || route.name }}
      </RouterLink>
    </nav>
  </header>

  <RouterView />
</template>

<style scoped>
header {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  width: 100%;
  height: 60px;
}
.main-nav {
  background-color: #666666;
  font-size: 1.8em;
}
</style>
