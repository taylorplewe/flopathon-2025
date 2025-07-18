<script setup>
import { RouterLink, RouterView, useRouter } from 'vue-router'

const router = useRouter()
const routes = router.options.routes
</script>

<template>
  <header>
    <nav class="main-nav">
      <div v-for="route in routes" :key="route.name">
        <RouterLink  :to="route.path" class="link">
          {{ route.meta?.displayName || route.name }}
        </RouterLink>|
      </div>
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
.link:hover {
  color: lime;
}
.main-nav {
  background-color: #666666;
  font-size: 1.8em;
  display: flex;
}
</style>
