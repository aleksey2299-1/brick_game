<script setup lang="ts">
import { onMounted, ref } from 'vue';
import { apiClient } from '../api/client';
import type { GamesList } from '../api/api';
import AppModal from './AppModal.vue';

const emit = defineEmits(['onGameChoose']);
const games = ref<GamesList | null>(null);

const showModal = ref<boolean>(false);

async function handleConfirm() {
  try {
    while (true) {
      await apiClient.actionsPost({ action_id: 2 });
      await apiClient.stateGet();
    }
  } catch (error) {
    showModal.value = false;
  }
}

function handleCancel() {
  showModal.value = false;
}

async function startGame(id?: number) {
  if (id !== undefined) {
    try {
      await apiClient.gamesGameIdPost(id);
      emit('onGameChoose');
    } catch (err: any) {
      if (err.status === 409) {
        showModal.value = true;
      } else {
        console.log('Ошибка запуска игры: ', err);
      }
    }
  }
}

onMounted(async () => {
  try {
    const response = await apiClient.gamesGet();
    games.value = response.data;
  } catch (err) {
    console.error('Не удалось получить список игр: ', err);
  }
});
</script>

<template>
  <div class="menu">
    <h2>Выбери игру</h2>
    <ul>
      <button class="shiny-btn" v-for="game in games?.games" @click="() => startGame(game.id)">
        <li>
          {{ game.name }}
        </li>
      </button>
    </ul>
  </div>
  <AppModal
    :show="showModal"
    message="Похоже кто-то забыл закончить игру (кто?). Помочь ему это сделать?"
    @confirm="handleConfirm"
    @cancel="handleCancel"
  />
</template>

<style scoped>
.menu {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
}
ul {
  display: flex;
  padding: 0;
  list-style-type: none;
  gap: 8px;
}
</style>
