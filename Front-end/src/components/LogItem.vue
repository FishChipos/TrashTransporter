<script setup lang="ts">
import type { ComputedRef } from 'vue';
import { computed } from 'vue';

export type LogTimestamp = {
    hours: number;
    minutes: number;
    seconds: number;
};

export type Log = {
    timestamp: LogTimestamp;
    content: string;
};

const props = defineProps<{
    log: Log
}>();

const timestampString: ComputedRef = computed(() => {
    const hoursString: string = ("0" + props.log.timestamp.hours).slice(-2);
    const minutesString: string = ("0" + props.log.timestamp.minutes).slice(-2);
    const secondsString: string = ("0" + props.log.timestamp.seconds).slice(-2);

    return hoursString + ":" + minutesString + ":" + secondsString;
})
</script>

<template>
    [{{ timestampString }}] {{ props.log.content }} 
    <br/>
</template>

<style scoped>

</style>