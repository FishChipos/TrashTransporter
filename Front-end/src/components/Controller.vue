<script setup lang="ts">
import { inject } from 'vue';
import type { Axios, AxiosError, AxiosResponse } from 'axios';

import buttonPressSoundURL from '../assets/button-press-sound.mp3';

const axios: Axios = inject('axios')!;

const props = defineProps<{
    isDisabled?: boolean
}>();

const buttonPressSound = new Audio(buttonPressSoundURL);
const buttonReleaseSound = new Audio(buttonPressSoundURL);
buttonPressSound.volume = 0.5;
buttonReleaseSound.volume = 0.5;

function move(direction: string, state: boolean) {
    if (state) {
        buttonPressSound.currentTime = 0.22;
        buttonPressSound.play();
    }
    else {
        buttonReleaseSound.currentTime = 0.22;
        buttonReleaseSound.play();
    }

    axios.get("/controls/move", {
        params: {
            direction: direction,
            state: state
        }
    })
    .catch((_error: AxiosError) => {});
}

</script>

<template>
    <div id="d-pad-container">
        <div/>
        <button :disabled="isDisabled" 
            @mousedown.left="move('forward', true)" 
            @mouseup.left="move('forward', false)"
        >
            FORWARD
        </button>
        <div/>
        <button :disabled="isDisabled"
            @mousedown.left="move('left', true)" 
            @mouseup.left="move('left', false)"
        >
            LEFT
        </button>
        <button :disabled="isDisabled"
            @mousedown.left="move('brake', true)"
            @mouseup.left="move('brake', false)"
        >
            BRAKE
        </button>
        <button :disabled="isDisabled"
            @mousedown.left="move('right', true)" 
            @mouseup.left="move('right', false)"
        >
            RIGHT
        </button>
        <div/>
        <button :disabled="isDisabled"
            @mousedown.left="move('back', true)" 
            @mouseup.left="move('back', false)"
        >
            BACK
        </button>
        <div/>
    </div>
</template>

<style scoped>
#d-pad-container {
    width: 100%;
    height: 100%;

    display: grid;

    gap: 0.5rem;
    grid: 1fr 1fr 1fr / 1fr 1fr 1fr;

    font-size: 12px;
}

button {
    width: 100%;
    height: 100%;

    border-radius: 0;

    transition: border-color 0.2s;

    overflow: hidden;
    word-break: keep-all;
}

button:hover:enabled {
    transition: border 0.2s, background-color 0.2s;
    background-color: var(--accent-color);
    border-color: var(--accent-color); 
}

button:active:enabled {
    transition: border 0.05s, background-color 0.05s;
    background-color: var(--main-color);
    border-color: var(--main-color);
}
</style>