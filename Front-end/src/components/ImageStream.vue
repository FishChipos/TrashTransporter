<script setup lang="ts">
import type { Ref } from 'vue';
import { ref } from 'vue';

const props = defineProps<{
    imageUrl: string,
    imageAlt?: string
}>();

const IMAGE_UPDATE_DELAY_ERROR = 5000;

const imageSrc: Ref<string> = ref("");

function imageLoad(): void {
    clearInterval(updateImageInterval);
}

function imageError(): void {
    clearInterval(updateImageInterval);
    updateImageInterval = setInterval(updateImage, IMAGE_UPDATE_DELAY_ERROR);
}

function updateImage(): void {
    imageSrc.value = props.imageUrl + "#" + new Date().getTime();
}

let updateImageInterval = setInterval(updateImage, IMAGE_UPDATE_DELAY_ERROR);

updateImage();

</script>

<template>
    <img :src="imageSrc" :alt="imageAlt" @error="imageError()" @load="imageLoad()"/>
</template>

<style scoped>
    img {
        object-position: center;

        object-fit: contain;

        max-width: 100%;

        justify-content: center;
        align-items: center;

        flex-grow: 1;
        flex-basis: 0;
    }
</style>