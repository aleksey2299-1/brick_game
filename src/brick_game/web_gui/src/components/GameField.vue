<script setup lang="ts">
import { onMounted, ref, onUnmounted } from 'vue';
import { apiClient } from '../api/client';
import type { GameState } from '../api/api';

const emit = defineEmits(['exitGame']);
const state = ref<GameState | undefined>();
const canvas = ref<HTMLCanvasElement | null>(null);

let isProcessing = false;
let hold = false;
let intervalId: number | undefined;
const cellSize = 20;
const padding = 8;

function drawField(ctx: CanvasRenderingContext2D, data?: Array<Array<boolean>>) {
  if (!ctx || !data || !data.length) return;
  const rows = data.length;
  const cols = data[0].length;
  for (let y = 0; y < rows; y++) {
    for (let x = 0; x < cols; x++) {
      ctx.fillStyle = data[y][x] ? 'rgb(60,60,150)' : 'lightgray';
      ctx.fillRect(x * cellSize, y * cellSize, cellSize - 1, cellSize - 1);
    }
  }
}

function drawNext(ctx: CanvasRenderingContext2D, data?: Array<Array<boolean>>) {
  if (!data || !data.length) return;
  const rows = data.length;
  const cols = data[0].length;
  for (let y = 0; y < rows; y++) {
    for (let x = 0; x < cols; x++) {
      ctx.fillStyle = data[y][x] ? 'rgb(200,60,150)' : 'lightgray';
      ctx.fillRect(11 * cellSize + x * cellSize, y * cellSize, cellSize - 1, cellSize - 1);
    }
  }
}

function drawLabel(ctx: CanvasRenderingContext2D, text: string, x: number, y: number) {
  ctx.font = 'bold 20px system-ui, Avenir, Helvetica, Arial, sans-serif';
  ctx.fillText(text, x, y);
}

function drawValue(ctx: CanvasRenderingContext2D, text: string, x: number, y: number) {
  ctx.font = '20px system-ui, Avenir, Helvetica, Arial, sans-serif';
  ctx.fillText(text, x, y);
}

function drawInfo(
  ctx: CanvasRenderingContext2D,
  level?: number,
  score?: number,
  highscore?: number,
  speed?: number
) {
  if (!ctx) return;
  const lineHeight = cellSize;
  let y = 5 * lineHeight;

  const gapBetweenLabels = lineHeight + 12;
  const gap = lineHeight + 8;

  ctx.fillStyle = 'black';
  ctx.textAlign = 'center';
  ctx.textBaseline = 'middle';
  ctx.strokeStyle = 'black';
  ctx.lineWidth = 3;
  const x = 13 * cellSize;
  drawLabel(ctx, 'Уровень', x, y);
  y += gap;
  drawValue(ctx, `${level}`, x, y);
  y += gapBetweenLabels;
  drawLabel(ctx, 'Рекорд', x, y);
  y += gap;
  drawValue(ctx, `${highscore}`, x, y);
  y += gapBetweenLabels;
  drawLabel(ctx, 'Очки', x, y);
  y += gap;
  drawValue(ctx, `${score}`, x, y);
  y += gapBetweenLabels;
  drawLabel(ctx, 'Скорость', x, y);
  y += gap;
  drawValue(ctx, `${speed} мс`, x, y);
}

function drawOverlay(
  ctx: CanvasRenderingContext2D,
  color: string,
  colorStroke: string,
  main: string,
  help: string
) {
  const startX = 5.6 * cellSize;
  const startY = 8 * cellSize;
  const startYHelp = startY + 2 * cellSize;
  ctx.font = 'bold 40px system-ui, Avenir, Helvetica, Arial, sans-serif';
  ctx.fillStyle = color;
  ctx.textAlign = 'center';
  ctx.textBaseline = 'middle';
  ctx.strokeStyle = colorStroke;
  ctx.strokeText(main, startX, startY);
  ctx.fillText(main, startX, startY);
  ctx.font = 'italic bold 20px system-ui, Avenir, Helvetica, Arial, sans-serif';
  ctx.strokeText(help, startX, startYHelp);
  ctx.fillText(help, startX, startYHelp);
}

function drawStatus(ctx: CanvasRenderingContext2D, state?: GameState) {
  if (!state?.speed) {
    drawOverlay(ctx, 'rgb(220,50,0)', 'black', 'GAMEOVER', "Press 'R' to start");
  }
  if (state?.level === -1) {
    drawOverlay(ctx, 'rgb(0,190,150)', 'white', 'VICTORY', "Press 'R' to start");
  }
  if (state?.pause) {
    drawOverlay(ctx, 'Black', 'white', 'PAUSED', "Press 'P' to continue");
  }
}

function drawGameState(state?: GameState) {
  if (!canvas.value) return;
  const ctx = canvas.value?.getContext('2d');
  if (!ctx) return;
  ctx.clearRect(0, 0, canvas.value.width, canvas.value.height);
  if (!state?.field?.length) {
    ctx.fillStyle = 'Purple';
    ctx.textAlign = 'center';
    ctx.textBaseline = 'middle';
    ctx.strokeStyle = 'black';
    ctx.font = 'italic 20px Arial';
    ctx.fillText("Press 'R' to start", cellSize * 8, cellSize * 5);
    ctx.fillText("Press 'SPACE' to exit", cellSize * 8, cellSize * 7);
  } else {
    drawField(ctx, state?.field);
    drawNext(ctx, state?.next);
    drawInfo(ctx, state?.level, state?.score, state?.high_score, state?.speed);
    drawStatus(ctx, state);
  }
}

function userAction(key: string) {
  switch (key) {
    case 'r':
    case 'R':
      return 0;
    case 'p':
    case 'P':
      return 1;
    case ' ':
      return 2;
    case 'ArrowLeft':
      return 3;
    case 'ArrowRight':
      return 4;
    case 'ArrowUp':
      return 5;
    case 'ArrowDown':
      return 6;
    case 'e':
    case 'E':
      return 7;
    default:
      return -1;
  }
}

async function handleKeydown(e: KeyboardEvent) {
  if (isProcessing) return;
  isProcessing = true;
  try {
    apiClient.actionsPost({ action_id: userAction(e.key), hold: hold });
    const response = await apiClient.stateGet();
    state.value = response.data;
    drawGameState(response.data);
  } catch (err) {
    console.error('Не удалось получить состояние игры: ', err);
  }
  setTimeout(() => {
    isProcessing = false;
    hold = true;
  }, 10);
}

async function handleKeyup() {
  if (isProcessing) return;
  isProcessing = true;
  try {
    apiClient.actionsPost({ action_id: -1, hold: false });
    const response = await apiClient.stateGet();
    state.value = response.data;
    drawGameState(response.data);
  } catch (err) {
    console.error('Не удалось получить состояние игры: ', err);
  }
  setTimeout(() => {
    isProcessing = false;
    hold = false;
  }, 10);
}

onMounted(async () => {
  if (canvas.value?.getContext('2d')) {
    const ctx = canvas.value.getContext('2d');
    if (ctx) {
      ctx.save();
      ctx.translate(padding, padding);
    }
  }
  intervalId = setInterval(async () => {
    try {
      const response = await apiClient.stateGet();
      state.value = response.data;
      drawGameState(response.data);
    } catch (err: any) {
      if (err.status == 400) emit('exitGame');
      console.error('Не удалось получить состояние игры: ', err);
    }
  }, 100);

  document.addEventListener('keydown', handleKeydown);
  document.addEventListener('keyup', handleKeyup);
});

onUnmounted(() => {
  if (intervalId !== undefined) {
    clearInterval(intervalId);
  }
  document.removeEventListener('keydown', handleKeydown);
  document.removeEventListener('keyup', handleKeyup);
});
</script>

<template>
  <div id="container">
    <canvas ref="canvas" width="316" height="416"></canvas>
    <div id="controls">
      <h2 id="control-title">Управление</h2>
      <span>
        <strong> Влево: </strong>
        ←
      </span>
      <span>
        <strong> Вправо: </strong>
        →
      </span>
      <span>
        <strong> Вниз: </strong>
        ↓
      </span>
      <span>
        <strong> Вверх: </strong>
        ↑
      </span>
      <span>
        <strong> Действие: </strong>
        E
      </span>
      <span>
        <strong> Пауза: </strong>
        P
      </span>
      <span>
        <strong> Остановить: </strong>
        SPACE
      </span>
    </div>
  </div>
</template>

<style scoped>
canvas {
  border: 3px double rgb(45, 123, 175);
  border-radius: 8px;
}
#container {
  display: flex;
  gap: 10px;
  width: max-content;
  text-align: start;
  height: min-content;
}
#controls {
  border: 3px double rgb(45, 123, 175);
  border-radius: 8px;
  padding: 8px;
  display: flex;
  flex-direction: column;
  align-items: start;
  width: max-content;
  justify-content: start;
}
#control-title {
  margin-top: 0;
}
</style>
