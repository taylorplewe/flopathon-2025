import { createRouter, createWebHistory } from 'vue-router'

const componentFiles = import.meta.glob('../components/*.vue')

const dynamicRoutes = Object.keys(componentFiles)
  .map((path) => {
    const componentName = path.split('/').pop().replace('.vue', '')

    if (componentName === 'HomeView') return null

    const routePath = '/' + componentName.replace(/([a-z0-9])([A-Z])/g, '$1-$2').toLowerCase()

    return {
      path: routePath,
      name: componentName.toLowerCase(),
      component: componentFiles[path],
      meta: {
        displayName: componentName.replace(/([A-Z])/g, ' $1').trim(),
      },
    }
  })
  .filter((route) => route !== null)

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),
  routes: [
    {
      path: '/',
      name: 'home',
      component: () => import('../components/HomeView.vue'),
      meta: {
        displayName: 'Home',
      },
    },
    ...dynamicRoutes,
  ],
})

export default router
