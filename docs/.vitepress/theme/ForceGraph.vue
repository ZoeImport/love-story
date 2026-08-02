<template>
  <div class="fg-wrap" ref="wrap">
    <canvas ref="canvas"></canvas>
    <div v-if="hovered" class="fg-tip" :style="tipStyle">{{ hovered.label }}</div>
  </div>
</template>

<script setup>
import { ref, onMounted, onBeforeUnmount } from 'vue'

const props = defineProps({
  nodes: { type: Array, required: true },   // { id, label, group, r }
  links: { type: Array, required: true },   // { source, target }
  colors: { type: Object, default: () => ({}) } // group -> color
})

const canvas = ref()
const wrap = ref()
const hovered = ref(null)
const tipStyle = ref({})

let ctx, raf, W, H
let pts = []      // 模拟点
let hoverId = null

// group -> 颜色
const palette = {
  skill: '#c05c42',
  knowledge: '#4a443a',
  book: '#948b7b',
  ...props.colors
}

function initGraph() {
  const ids = new Map()
  pts = props.nodes.map((n, i) => {
    ids.set(n.id, i)
    const a = (i / props.nodes.length) * Math.PI * 2
    return {
      ...n,
      x: W / 2 + Math.cos(a) * Math.min(W, H) * 0.28,
      y: H / 2 + Math.sin(a) * Math.min(W, H) * 0.28,
      vx: 0, vy: 0,
      r: n.r || (n.group === 'skill' ? 26 : 18)
    }
  })
  // links -> index pairs
  const edges = props.links
    .map(l => {
      const s = ids.get(l.source), t = ids.get(l.target)
      return s !== undefined && t !== undefined ? [s, t] : null
    })
    .filter(Boolean)
  return edges
}

function physics(edges) {
  const repK = 1800, springK = 0.02, rest = 150, damp = 0.86
  for (let i = 0; i < pts.length; i++) {
    for (let j = i + 1; j < pts.length; j++) {
      const a = pts[i], b = pts[j]
      let dx = b.x - a.x, dy = b.y - a.y
      let d2 = dx * dx + dy * dy
      if (d2 < 1) { d2 = 1; dx = Math.random() - 0.5; dy = Math.random() - 0.5 }
      const d = Math.sqrt(d2)
      const f = repK / d2
      const fx = (dx / d) * f, fy = (dy / d) * f
      a.vx -= fx; a.vy -= fy
      b.vx += fx; b.vy += fy
    }
  }
  for (const [s, t] of edges) {
    const a = pts[s], b = pts[t]
    const dx = b.x - a.x, dy = b.y - a.y
    const d = Math.sqrt(dx * dx + dy * dy) || 1
    const f = (d - rest) * springK
    const fx = (dx / d) * f, fy = (dy / d) * f
    a.vx += fx; a.vy += fy
    b.vx -= fx; b.vy -= fy
  }
  for (const p of pts) {
    p.vx *= damp; p.vy *= damp
    p.x += p.vx; p.y += p.vy
    p.x = Math.max(40, Math.min(W - 40, p.x))
    p.y = Math.max(40, Math.min(H - 40, p.y))
  }
}

function draw(edges) {
  ctx.clearRect(0, 0, W, H)
  // edges
  ctx.lineWidth = 1
  for (const [s, t] of edges) {
    const a = pts[s], b = pts[t]
    const hot = hoverId === a.id || hoverId === b.id
    ctx.strokeStyle = hot ? 'rgba(192, 92, 66, 0.5)' : 'rgba(90, 80, 66, 0.16)'
    ctx.beginPath(); ctx.moveTo(a.x, a.y); ctx.lineTo(b.x, b.y); ctx.stroke()
  }
  // nodes
  for (const p of pts) {
    const isHover = hoverId === p.id
    const isNeighbor = isHover || (hoverId !== null && edges.some(([s, t]) =>
      (pts[s].id === hoverId && pts[t].id === p.id) || (pts[t].id === hoverId && pts[s].id === p.id)))
    const c = palette[p.group] || '#888'
    ctx.globalAlpha = hoverId === null || isNeighbor ? 1 : 0.28
    if (p.group === 'skill') {
      // 陶土描边圆 + 细环
      ctx.beginPath(); ctx.arc(p.x, p.y, p.r, 0, Math.PI * 2)
      ctx.strokeStyle = isHover ? c : c
      ctx.lineWidth = isHover ? 2 : 1.2
      ctx.stroke()
      ctx.beginPath(); ctx.arc(p.x, p.y, p.r - 4, 0, Math.PI * 2)
      ctx.fillStyle = isHover ? 'rgba(192,92,66,0.12)' : 'rgba(192,92,66,0.06)'
      ctx.fill()
    } else {
      ctx.beginPath(); ctx.arc(p.x, p.y, p.r * 0.6, 0, Math.PI * 2)
      ctx.fillStyle = c
      ctx.globalAlpha = ctx.globalAlpha * 0.7
      ctx.fill()
    }
    ctx.globalAlpha = 1
    // label
    ctx.font = '13px "Noto Sans SC", sans-serif'
    ctx.textAlign = 'center'
    ctx.fillStyle = hoverId === p.id ? '#c05c42' : 'rgba(44,40,35,0.85)'
    ctx.fillText(p.label, p.x, p.y + p.r + 16)
  }
}

function loop(edges) {
  physics(edges)
  draw(edges)
  raf = requestAnimationFrame(() => loop(edges))
}

function onMove(e) {
  const rect = canvas.value.getBoundingClientRect()
  const x = e.clientX - rect.left, y = e.clientY - rect.top
  let found = null, minD = 50
  for (const p of pts) {
    const d = Math.hypot(p.x - x, p.y - y)
    if (d < minD) { minD = d; found = p }
  }
  hoverId = found ? found.id : null
  hovered.value = found
  if (found) {
    const rect2 = wrap.value.getBoundingClientRect()
    tipStyle.value = {
      left: Math.min(x + 14, rect2.width - 140) + 'px',
      top: (y - 10) + 'px'
    }
  }
}
function onLeave() { hoverId = null; hovered.value = null }

onMounted(() => {
  const dpr = window.devicePixelRatio || 1
  const rect = wrap.value.getBoundingClientRect()
  W = rect.width; H = Math.max(340, rect.height)
  canvas.value.width = W * dpr
  canvas.value.height = H * dpr
  canvas.value.style.width = W + 'px'
  canvas.value.style.height = H + 'px'
  ctx = canvas.value.getContext('2d')
  ctx.scale(dpr, dpr)
  const edges = initGraph()
  // warm-up physics
  for (let i = 0; i < 90; i++) physics(edges)
  loop(edges)
})
onBeforeUnmount(() => cancelAnimationFrame(raf))
</script>

<style scoped>
.fg-wrap { position: relative; width: 100%; min-height: 360px; }
.fg-wrap canvas { display: block; width: 100%; height: 100%; cursor: default; }
.fg-tip {
  position: absolute; pointer-events: none;
  background: #1e1b16; color: #f8f3ea;
  font-size: 12px; padding: 4px 10px; border-radius: 8px;
  box-shadow: 0 2px 8px rgba(0,0,0,0.18);
  z-index: 5; white-space: nowrap;
}
</style>
