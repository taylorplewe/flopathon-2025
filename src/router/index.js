import { createRouter, createWebHistory } from 'vue-router'

// Dynamically import all Vue components from the components directory
// This uses Vite's feature to import multiple modules at once
const componentFiles = import.meta.glob('../components/*.vue')

// Generate routes dynamically from component files
const dynamicRoutes = Object.keys(componentFiles)
  .map((path) => {
    // Extract component name from path (e.g., "../components/TeamName.vue" -> "TeamName")
    const componentName = path.split('/').pop().replace('.vue', '')

    // Skip HomeView as it has a special route
    if (componentName === 'HomeView') return null

    // Convert component name to route path (e.g., "TeamName" -> "/team-name")
    // This follows standard kebab-case URL convention
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

// Create the router with home route and dynamic routes
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
    // Spread the dynamically generated routes
    ...dynamicRoutes,
  ],
})

export default router
