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

2. Add your own route in `src/router/index.js`

```js
// Example of adding a new route
{
  path: '/your-team-name',
  name: 'yourTeamName',
  component: () => import('../components/YourTeamComponent.vue'),
}
```

3. Create your team's Vue component in `src/components/`

4. Submit a pull request when your feature is complete