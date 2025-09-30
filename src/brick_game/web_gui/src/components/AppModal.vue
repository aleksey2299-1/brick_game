<script setup lang="ts">
defineProps({
  show: Boolean,
  title: { type: String, default: 'Что-то пошло не так' },
  message: { type: String, default: 'Сделать что-нибудь?' },
});

defineEmits(['confirm', 'cancel']);
</script>

<template>
  <teleport to="body">
    <div v-if="show" class="overlay">
      <div class="modal">
        <h2>{{ title }}</h2>
        <p>{{ message }}</p>
        <div class="actions">
          <button @click="$emit('cancel')">Отмена</button>
          <button @click="$emit('confirm')">Подтвердить</button>
        </div>
      </div>
    </div>
  </teleport>
</template>

<style scoped>
.overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(0, 0, 0, 0.5);
  display: flex;
  align-items: center;
  justify-content: center;
  z-index: 9999;
}

.modal {
  background: white;
  padding: 20px;
  border-radius: 8px;
  width: 20%;
  max-width: 90%;
  box-shadow: 0 4px 12px rgba(0, 0, 0, 0.3);
  animation: appear 0.3s ease-in;
}

.actions {
  display: flex;
  justify-content: flex-end;
  gap: 10px;
  margin-top: 20px;
}

@keyframes appear {
  0% {
    transform: translateY(100px);
    opacity: 0;
  }
  100% {
    transform: translateY(0);
    opacity: 1;
  }
}
</style>
