<script setup lang="ts">
import { ref } from 'vue';
import AppMenu from './components/AppMenu.vue';
import GameField from './components/GameField.vue';

const playing = ref<boolean>(false);

function handleGameChoose() {
  playing.value = true;
}

function handleExitGame() {
  playing.value = false;
}
</script>

<template>
  <div id="game">
    <h1 id="game-name">Brick game</h1>
    <AppMenu v-if="!playing" @onGameChoose="handleGameChoose" />
    <GameField v-if="playing" @exitGame="handleExitGame" />
  </div>
</template>

<style scoped>
#game {
  width: min-content;
  min-height: 400px;
  padding-bottom: 200px;
}
#game-name {
  background: linear-gradient(135deg, red, orange, green, indigo, violet);
  -webkit-background-clip: text;
  background-clip: text;
  color: transparent;
}
</style>
