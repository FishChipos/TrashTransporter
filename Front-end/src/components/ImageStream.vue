<script setup lang="ts">
import type { Ref } from 'vue';
import { ref } from 'vue';

const props = defineProps<{
    imageUrl: string,
    imageAlt?: string
}>();

const IMAGE_UPDATE_DELAY = 1000;
const IMAGE_UPDATE_DELAY_ERROR = 5000;

const imageSrc: Ref<string> = ref("");

function imageLoad(): void {
    clearInterval(updateImageInterval);
    updateImageInterval = setInterval(updateImage, IMAGE_UPDATE_DELAY);
}

function imageError(): void {
    clearInterval(updateImageInterval);
    updateImageInterval = setInterval(updateImage, IMAGE_UPDATE_DELAY_ERROR);
}

function updateImage(): void {
    imageSrc.value = props.imageUrl + "#" + new Date().getTime();
}

let updateImageInterval = setInterval(updateImage, IMAGE_UPDATE_DELAY);

</script>

<template>
    <img :src="imageSrc" :alt="imageAlt" @error="imageError()" @load="imageLoad()"/>
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