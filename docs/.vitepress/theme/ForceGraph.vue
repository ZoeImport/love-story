<template>
  <div class="fg-wrap" ref="wrap">
    <canvas ref="canvas" @mousemove="onMove" @mouseleave="onLeave" @click="onClick"></canvas>
    <div v-if="hovered" class="fg-tip" :style="tipStyle">{{ hovered.label }}</div>
  </div>
</template>

<script setup>
import { ref, onMounted, onBeforeUnmount } from 'vue'

const props = defineProps({
  nodes: { type: Array, required: true },
  links: { type: Array, required: true },
  colors: { type: Object, default: () => ({}) }
})
const emit = defineEmits(['select'])

const canvas = ref()
const wrap = ref()
const hovered = ref(null)
const tipStyle = ref({})

let ctx, W, H
let pts = []
let edges = []
let hoverId = null

const palette = { skill: '#c05c42', knowledge: '#4a443a', ...props.colors }
const CAT_COLORS = {
  'Linux 内核': '#66796b',
  '网络': '#6a86a6',
  '代理': '#b06a4b',
  default: '#948b7b'
}

function layout() {
  pts = props.nodes.map((n) => ({ ...n, x: 0, y: 0, r: n.group === 'skill' ? 22 : 16 }))
  const skills = pts.filter((n) => n.group === 'skill')
  const knows = pts.filter((n) => n.group === 'knowledge')
  const R1 = Math.min(W, H) * 0.16
  const R2 = Math.min(W, H) * 0.34
  skills.forEach((n, i) => {
    const a = (i / Math.max(skills.length, 1)) * Math.PI * 2 - Math.PI / 2
    n.x = W / 2 + Math.cos(a) * R1
    n.y = H / 2 + Math.sin(a) * R1
  })
  const cats = [...new Set(knows.map((k) => k.category))]
  knows.forEach((n, i) => {
    const ci = cats.indexOf(n.category)
    const spread = Math.PI * 2 / cats.length
    const a = ci * spread + spread / 2 + (i % 3 - 1) * 0.14
    n.x = W / 2 + Math.cos(a) * R2
    n.y = H / 2 + Math.sin(a) * R2
  })
  const idMap = new Map(pts.map((p) => [p.id, p]))
  edges = props.links
    .map((l) => [idMap.get(l.source), idMap.get(l.target)])
    .filter((e) => e[0] && e[1])
}

function draw() {
  ctx.clearRect(0, 0, W, H)
  ctx.lineWidth = 1
  for (const [a, b] of edges) {
    const hot = hoverId === a.id || hoverId === b.id
    ctx.strokeStyle = hot ? 'rgba(192,92,66,0.55)' : 'rgba(90,80,66,0.18)'
    ctx.beginPath(); ctx.moveTo(a.x, a.y); ctx.lineTo(b.x, b.y); ctx.stroke()
  }
  for (const p of pts) {
    const isHover = hoverId === p.id
    const isNeighbor = hoverId !== null && edges.some(([a, b]) =>
      (a.id === hoverId && b.id === p.id) || (b.id === hoverId && a.id === p.id))
    ctx.globalAlpha = hoverId === null || isHover || isNeighbor ? 1 : 0.25
    if (p.group === 'skill') {
      ctx.beginPath(); ctx.arc(p.x, p.y, p.r, 0, Math.PI * 2)
      ctx.fillStyle = isHover ? 'rgba(192,92,66,0.16)' : 'rgba(192,92,66,0.07)'
      ctx.fill()
      ctx.strokeStyle = palette.skill
      ctx.lineWidth = isHover ? 2 : 1.2
      ctx.stroke()
    } else {
      const c = CAT_COLORS[p.category] || CAT_COLORS.default
      ctx.beginPath(); ctx.arc(p.x, p.y, p.r * 0.62, 0, Math.PI * 2)
      ctx.fillStyle = c
      ctx.fill()
      ctx.strokeStyle = isHover ? palette.skill : 'rgba(90,80,66,0.5)'
      ctx.lineWidth = isHover ? 2 : 1
      ctx.stroke()
    }
    ctx.globalAlpha = 1
    ctx.font = isHover ? '600 14px "Noto Sans SC", sans-serif' : '400 12.5px "Noto Sans SC", sans-serif'
    ctx.textAlign = 'center'
    ctx.fillStyle = isHover ? palette.skill : 'rgba(44,40,35,0.85)'
    ctx.fillText(p.label, p.x, p.y + p.r + 16)
  }
  // center dot
  ctx.beginPath(); ctx.arc(W / 2, H / 2, 3.5, 0, Math.PI * 2)
  ctx.fillStyle = 'rgba(44,40,35,0.6)'
  ctx.fill()
}

function posFromEvent(e) {
  const rect = canvas.value.getBoundingClientRect()
  return [e.clientX - rect.left, e.clientY - rect.top]
}
function hit(x, y) {
  let found = null, minD = 40
  for (const p of pts) {
    const d = Math.hypot(p.x - x, p.y - y)
    if (d < minD) { minD = d; found = p }
  }
  return found
}
function onMove(e) {
  const [x, y] = posFromEvent(e)
  const f = hit(x, y)
  hoverId = f ? f.id : null
  hovered.value = f
  if (f) {
    const r = wrap.value.getBoundingClientRect()
    tipStyle.value = { left: Math.min(x + 14, r.width - 150) + 'px', top: y - 8 + 'px' }
  }
  draw()
}
function onLeave() { hoverId = null; hovered.value = null; draw() }
function onClick(e) {
  const [x, y] = posFromEvent(e)
  const f = hit(x, y)
  if (f) emit('select', f)
}

onMounted(() => {
  const dpr = window.devicePixelRatio || 1
  const rect = wrap.value.getBoundingClientRect()
  W = rect.width; H = Math.max(360, rect.height)
  canvas.value.width = W * dpr
  canvas.value.height = H * dpr
  canvas.value.style.width = W + 'px'
  canvas.value.style.height = H + 'px'
  ctx = canvas.value.getContext('2d')
  ctx.scale(dpr, dpr)
  layout()
  draw()
})
</script>

<style scoped>
.fg-wrap { position: relative; width: 100%; min-height: 380px; }
.fg-wrap canvas { display: block; width: 100%; height: 100%; cursor: pointer; }
.fg-tip {
  position: absolute; pointer-events: none;
  background: #1e1b16; color: #f8f3ea;
  font-size: 12px; padding: 4px 10px; border-radius: 8px;
  box-shadow: 0 2px 8px rgba(0,0,0,0.18);
  z-index: 5; white-space: nowrap;
}
</style>
