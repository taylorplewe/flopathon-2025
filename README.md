# Flopathon-2025

This repository is for Awardco's 2025 Flopathon. It contains a Vue 3 project powered by Vite with Vue Router, Pinia state management, and Vitest for unit testing.

## Recommended IDE Setup

[VSCode](https://code.visualstudio.com/) + [Volar](https://marketplace.visualstudio.com/items?itemName=Vue.volar) (and disable Vetur).

## Customize configuration

See [Vite Configuration Reference](https://vite.dev/config/).

## Getting Started

1. Clone the repository

```sh
git clone https://github.com/colefin8/flopathon-2025.git
cd flopathon-2025
```

2. Install dependencies

```sh
npm install
```

3. Start the development server

```sh
npm run dev
```

## Contributing

Each team should follow these steps to contribute:

1. Create a branch off of master with your team name

```sh
git checkout -b team-name
```

2. Rename the example route in `src/router/index.js` to match your team name

```js
// Change this route to match your team name
{
  path: '/example', // Change to your team name (e.g., '/team-awesome')
  name: 'example',   // Change to your team name (e.g., 'teamAwesome')
  component: () => import('../components/RenameMe.vue'), // Update this path if you rename the component
}
```

3. Rename the file of the example component in `src/components/`

4. Update the RouterLink in `src/App.vue` to point to your team's page

```vue
<!-- Change this RouterLink to match your team name -->
<RouterLink to="/example">Example</RouterLink>
<!-- Change to your team name -->
```

5. Open a pull request so we can merge them all in for presenting.
