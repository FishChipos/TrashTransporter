<script setup lang="ts">
import { useSlots } from 'vue';

const props = defineProps<{
    scrollable?: boolean,
    noPadding?: boolean,
}>();

const slots = useSlots();

const showTitle = !!slots["title"];

const overflowY = props.scrollable ? "scroll" : "hidden";
const padding = props.noPadding ? "0" : "1rem";
</script>

<template>
    <div id="container">
        <div id="title" v-if="showTitle">
            <b><slot name="title"/></b>
        </div>
        <div id="body">
            <slot name="body">

            </slot>
        </div>
    </div>
</template>

<style scoped>
#container {
    display: flex;
    flex-direction: column;

    height: 100%;
}

#title {
    color: var(--main-bg-color);
    background-color: var(--main-color);

    width: 100%;

    text-align: center;

    padding-top: 0.5rem;
    padding-bottom: 0.5rem;
}

#body {
    flex-grow: 1;

    padding: v-bind("padding");

    overflow-y: v-bind("overflowY");
    word-break: break-all;
    scrollbar-width: thin;
}
</style>