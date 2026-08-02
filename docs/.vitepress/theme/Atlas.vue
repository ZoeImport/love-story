<template>
  <div class="atlas">
    <header class="atlas-hero">
      <span class="eyebrow">Cache / Knowledge Atlas</span>
      <h1>知识图谱<em class="dot">.</em></h1>
      <p>技能、知识条目与书籍的关联地图，以及 Claude Code 的真实 skill 使用。点击任何条目展开完整讲解。</p>
    </header>

    <!-- ① 技能图谱 -->
    <section class="atlas-section" aria-labelledby="atlas-graph-title">
      <div class="atlas-sec-head">
        <h2 id="atlas-graph-title">技能图谱</h2>
        <p class="hint">陶土 = 技能 · 彩色 = 知识条目（按分类分扇区）。点击节点联动下方详情。</p>
      </div>
      <ForceGraph :nodes="graph.nodes" :links="graph.links" @select="onGraphSelect" />
    </section>

    <!-- ② 知识条目（可点击展开详情） -->
    <section class="atlas-section" id="entries" aria-labelledby="atlas-know-title">
      <div class="atlas-sec-head">
        <h2 id="atlas-know-title">知识条目</h2>
        <p class="hint">每个条目含「何时引入 · 背景概念 · 详细讲解 · 子关键词」。点击展开。</p>
      </div>
      <input v-model="query" class="atlas-search" type="search" placeholder="搜索 eBPF、iptables、clash、fakeip…" aria-label="搜索知识条目" />
      <p class="atlas-count">{{ filteredKnowledge.length }} 条</p>
      <div class="atlas-list">
        <article v-for="k in filteredKnowledge" :key="k.id"
                 class="atlas-card" :class="{ 'is-open': selected?.id === k.id }"
                 @click="select(k)">
          <span class="atlas-cat">{{ k.category }}</span>
          <h3>{{ k.title }}</h3>
          <p>{{ k.summary }}</p>
          <div class="atlas-tags"><span v-for="t in k.tags" :key="t">{{ t }}</span></div>
        </article>
      </div>

      <!-- 详情面板 -->
      <div v-if="selected" class="atlas-detail" ref="detailEl">
        <button class="atlas-detail-close" aria-label="关闭详情" @click="selected = null">×</button>
        <span class="atlas-cat">{{ selected.category }}</span>
        <h3>{{ selected.title }}</h3>
        <p class="atlas-detail-summary">{{ selected.summary }}</p>

        <h4>何时引入</h4>
        <p>{{ selected.introduced }}</p>
        <h4>背景概念</h4>
        <p>{{ selected.background }}</p>
        <h4>详细讲解</h4>
        <p>{{ selected.detail }}</p>
        <h4>子关键词</h4>
        <ul class="atlas-subtopics">
          <li v-for="st in selected.subTopics" :key="st.name"><strong>{{ st.name }}</strong><span> — {{ st.note }}</span></li>
        </ul>
        <p v-if="relatedBooks(selected).length" class="atlas-books-link">
          相关书籍：
          <a v-for="b in relatedBooks(selected)" :key="b.id" href="#books" @click.prevent="scrollToBooks">{{ b.title }}</a>
        </p>
      </div>
    </section>

    <!-- ③ 阅读（书籍独立块） -->
    <section class="atlas-section" id="books" aria-labelledby="atlas-books-title">
      <div class="atlas-sec-head">
        <h2 id="atlas-books-title">阅读</h2>
        <p class="hint">在读与推荐，进度见卡片标注。</p>
      </div>
      <div class="atlas-books">
        <div class="atlas-books-col">
          <h3>在读</h3>
          <article v-for="b in books.reading" :key="b.id" class="atlas-card">
            <h4>{{ b.title }}</h4>
            <p v-if="b.progress" class="atlas-progress">{{ b.progress }}</p>
            <p>{{ b.detail }}</p>
          </article>
        </div>
        <div class="atlas-books-col">
          <h3>推荐</h3>
          <article v-for="b in books.recommended" :key="b.id" class="atlas-card">
            <h4>{{ b.title }}</h4>
            <p>{{ b.detail }}</p>
          </article>
        </div>
      </div>
    </section>

    <!-- ④ Claude Code skill 使用 -->
    <section class="atlas-section" aria-labelledby="atlas-usage-title">
      <div class="atlas-sec-head">
        <h2 id="atlas-usage-title">Claude Code skill 使用</h2>
        <p class="hint">来源：{{ usage.source }} · {{ usage.window }}</p>
      </div>
      <div class="usage-list">
        <div v-for="u in usage.skills" :key="u.name" class="usage-row">
          <span class="usage-name">{{ u.name }}</span>
          <span class="usage-bar"><i :style="{ width: barWidth(u.invocations) }"></i></span>
          <span class="usage-count">{{ u.invocations }}</span>
        </div>
      </div>
      <p class="atlas-footnote">{{ usage.note }}</p>
    </section>
  </div>
</template>

<script setup>
import { ref, computed, nextTick } from 'vue'
import ForceGraph from './ForceGraph.vue'
import booksData from '../../atlas/data/books.json'
import knowledgeData from '../../atlas/data/knowledge.json'
import skillsData from '../../atlas/data/skills.json'
import usageData from '../../atlas/data/usage.json'

const books = booksData
const knowledge = knowledgeData
const skills = skillsData.skills
const usage = usageData

const query = ref('')
const selected = ref(null)
const detailEl = ref(null)

const filteredKnowledge = computed(() => {
  const q = query.value.trim().toLowerCase()
  if (!q) return knowledge
  return knowledge.filter((k) =>
    (k.title + k.summary + k.tags.join(' ') + k.category).toLowerCase().includes(q)
  )
})

// 图谱：技能(内圈) + 知识条目(外圈)，books 独立不进入图谱
const kMap = new Map(knowledge.map((k) => [k.id, k]))
const allBooks = [...books.reading, ...books.recommended]
const graph = computed(() => {
  const nodes = [
    ...skills.map((s) => ({ id: s.id, label: s.label, group: 'skill' })),
    ...knowledge.map((k) => ({ id: k.id, label: k.title, group: 'knowledge', category: k.category }))
  ]
  const links = []
  skills.forEach((s) => {
    ;(s.knowledge || []).forEach((kid) => {
      if (kMap.has(kid)) links.push({ source: s.id, target: kid })
    })
  })
  return { nodes, links }
})

function onGraphSelect(node) {
  if (node.group === 'knowledge') {
    select(kMap.get(node.id))
  }
}
function select(k) {
  selected.value = k
  nextTick(() => {
    detailEl.value?.scrollIntoView({ behavior: 'smooth', block: 'nearest' })
  })
}
function relatedBooks(k) {
  return (k.books || []).map((bid) => allBooks.find((b) => b.id === bid)).filter(Boolean)
}
function scrollToBooks() {
  document.getElementById('books')?.scrollIntoView({ behavior: 'smooth' })
}

const maxInv = Math.max(...usage.skills.map((u) => u.invocations))
function barWidth(inv) {
  return Math.max(6, Math.round((inv / maxInv) * 100)) + '%'
}
</script>

<style scoped>
.atlas { padding-bottom: 64px; }

.atlas-hero { padding: 72px 0 48px; border-bottom: 1px solid var(--border-faint); margin-bottom: 48px; }
.atlas-hero .eyebrow { font-size: 12px; letter-spacing: 0.12em; text-transform: uppercase; color: var(--clay-deep); }
.atlas-hero h1 { font-family: var(--font-serif); font-size: clamp(34px, 5vw, 48px); font-weight: 600; line-height: 1.1; margin: 12px 0 16px; letter-spacing: -0.015em; }
.atlas-hero .dot { color: var(--clay); font-style: normal; }
.atlas-hero p { color: var(--text-muted); max-width: 60ch; line-height: 1.75; }

.atlas-section { margin-bottom: 72px; scroll-margin-top: 40px; }
.atlas-sec-head { display: flex; align-items: baseline; gap: 16px; flex-wrap: wrap; margin-bottom: 20px; }
.atlas-sec-head h2 { font-family: var(--font-serif); font-size: 24px; font-weight: 600; margin: 0; }
.atlas-sec-head .hint { color: var(--text-faint); font-size: 13px; margin: 0; }

.atlas-search { width: 100%; max-width: 480px; background: var(--paper-card); border: 1px solid var(--border-base); border-radius: var(--radius-btn); padding: 10px 14px; font-size: 15px; color: var(--text-body); outline: none; transition: border-color var(--dur-base) var(--ease-out-quart); }
.atlas-search:focus { border-color: var(--clay); }
.atlas-count { color: var(--text-faint); font-size: 12px; margin: 10px 0 0; }

.atlas-list { display: grid; grid-template-columns: repeat(auto-fill, minmax(280px, 1fr)); gap: 14px; margin-top: 18px; }
.atlas-card { background: var(--paper-card); border: 1px solid var(--border-faint); border-radius: var(--radius-md); padding: 18px 20px; cursor: pointer; transition: border-color var(--dur-base) var(--ease-out-quart), box-shadow var(--dur-base) var(--ease-out-quart); }
.atlas-card:hover { border-color: var(--border-strong); }
.atlas-card.is-open { border-color: var(--clay); box-shadow: 0 0 0 1px var(--clay); }
.atlas-card h3, .atlas-card h4 { margin: 6px 0 8px; font-family: var(--font-serif); font-weight: 600; }
.atlas-card p { margin: 0 0 8px; color: var(--text-body); font-size: 14px; line-height: 1.65; }
.atlas-cat { display: inline-block; font-size: 11px; letter-spacing: 0.06em; color: var(--clay-deep); background: var(--clay-tint); padding: 2px 8px; border-radius: 999px; }
.atlas-progress { color: var(--clay-deep) !important; font-size: 13px !important; }
.atlas-tags { display: flex; flex-wrap: wrap; gap: 6px; }
.atlas-tags span { font-size: 11px; color: var(--text-muted); border: 1px solid var(--border-base); border-radius: 999px; padding: 1px 8px; }

.atlas-detail { position: relative; margin-top: 22px; padding: 26px 28px; background: var(--paper-card); border: 1px solid var(--clay); border-radius: var(--radius-lg); scroll-margin-top: 40px; }
.atlas-detail-close { position: absolute; top: 12px; right: 14px; border: none; background: none; font-size: 22px; line-height: 1; color: var(--text-muted); cursor: pointer; }
.atlas-detail-close:hover { color: var(--clay-deep); }
.atlas-detail h3 { font-family: var(--font-serif); font-size: 26px; font-weight: 600; margin: 8px 0 10px; }
.atlas-detail-summary { color: var(--text-muted); font-size: 15px; }
.atlas-detail h4 { font-family: var(--font-serif); font-size: 16px; font-weight: 600; margin: 20px 0 6px; color: var(--clay-deep); }
.atlas-detail p { color: var(--text-body); font-size: 14.5px; line-height: 1.75; margin: 0 0 8px; }
.atlas-subtopics { list-style: none; padding: 0; margin: 0; }
.atlas-subtopics li { padding: 8px 0; border-top: 1px solid var(--border-faint); font-size: 14px; color: var(--text-body); line-height: 1.65; }
.atlas-subtopics strong { color: var(--text-strong); }
.atlas-books-link { margin-top: 18px !important; }
.atlas-books-link a { color: var(--clay-deep); text-decoration: underline; text-underline-offset: 3px; margin-right: 14px; }

.atlas-books { display: grid; grid-template-columns: 1fr 1fr; gap: 24px; }
.atlas-books-col > h3 { font-family: var(--font-serif); font-size: 18px; margin: 0 0 12px; }
.atlas-books-col .atlas-card { margin-bottom: 12px; }
@media (max-width: 720px) { .atlas-books { grid-template-columns: 1fr; } }

.usage-list { margin-top: 8px; }
.usage-row { display: grid; grid-template-columns: 180px 1fr 56px; gap: 12px; align-items: center; padding: 7px 0; }
.usage-name { font-size: 13px; color: var(--text-body); font-family: var(--font-mono); }
.usage-bar { background: var(--paper-deep); border-radius: 999px; height: 8px; overflow: hidden; }
.usage-bar i { display: block; height: 100%; background: var(--clay); border-radius: 999px; }
.usage-count { font-size: 13px; color: var(--text-muted); text-align: right; font-variant-numeric: tabular-nums; }
.atlas-footnote { color: var(--text-faint); font-size: 12px; margin-top: 12px; }
</style>
