import DefaultTheme from 'vitepress/theme'
import Atlas from './Atlas.vue'
import './custom.css'

export default {
  ...DefaultTheme,
  enhanceApp({ app }) {
    app.component('KnowledgeAtlas', Atlas)
  }
}
