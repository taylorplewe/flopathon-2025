import { createRouter, createWebHistory } from 'vue-router'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      name: 'home',
      component: () => import('../components/HomeView.vue'),
    },
    {
      path: '/example',
      name: 'example',
      component: () => import('../components/RenameMe.vue'),
    },
    //add a route here for your group, mirroring the structure above
  ],
})

export default router
