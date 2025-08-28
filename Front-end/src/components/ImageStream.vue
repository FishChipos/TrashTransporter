<script setup lang="ts">
import type { Ref } from 'vue';
import { ref } from 'vue';

const props = defineProps<{
    imageUrl: string,
    imageAlt?: string
}>();

const imageSrc: Ref<string> = ref("");

function updateImage(): void {
    imageSrc.value = props.imageUrl + "#" + new Date().getTime();
}

setInterval(updateImage, 1000);

</script>

<template>
    <img :src="imageSrc" :alt="imageAlt"/>
</template>

<style scoped>
    img {
        height: 100%;
        object-fit: contain;
        display: flex;
        justify-content: space-evenly;
        align-items: center;
    }
</style>