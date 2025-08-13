<script setup lang="ts">
import type { Axios, AxiosError, AxiosResponse } from 'axios';
import type { Reactive, Ref } from 'vue';
import { inject, reactive, ref } from 'vue';

import type { Log } from './components/LogItem.vue';
import LogItem from './components/LogItem.vue';

import Container from './components/Container.vue';

const axios: Axios = inject('axios')!;
axios.defaults.baseURL = "http://esp32.local";
axios.defaults.withCredentials = false;

let isLogsAccessible: Ref<boolean> = ref(false);
let isManualControl: Ref<boolean> = ref(false);

const logs: Reactive<Log[]> = reactive([]);
let logCount: number = 0;

function readFromServer(): void {
    // Get logs.
    axios.get("/logs", {
        timeout: 5000,
    })
    .then((response: AxiosResponse) => {
        isLogsAccessible.value = true;
        for (let log = logCount; log < response.data.count; log++) {
            logs.push(response.data.logs[log]);
            logCount = log + 1;
        }
    })
    .catch((_error: AxiosError) => {
        isLogsAccessible.value = false;
    });
}

setInterval(readFromServer, 1000);

function toggleManualControl(): void {
    isManualControl.value = !isManualControl.value;
}

</script>

<template>
    <div id="img-output-container">
        <div class="img-container">
            <img src="http://esp32.local/output/raw" alt="raw image output" />
        </div>
        <div class="img-container">
            <img src="http://esp32.local/output/marked" alt="marked image output" />
        </div>
    </div>
    <div id="user-container">
        <Container>
            <template v-slot:title>SETTINGS</template>
            <template v-slot:body>
                <input type="checkbox" @click="toggleManualControl()">Manual Control</input>
            </template>
        </Container>
        <Container>

        </Container>
        <Container :scrollable="true">
            <template v-slot:title>
                <template v-if="isLogsAccessible">
                    LOGS
                </template>
                <template v-else>
                    LOGS NOT ACCESSIBLE
                </template>
            </template>
            <template v-slot:body v-if="isLogsAccessible">
                <LogItem
                    v-for="log in logs"
                    :log="log"
                />
            </template>
        </Container>
    </div>
</template>

<style scoped>
/* Excuse the horrendous CSS. */
#img-output-container {
    display: flex;

    flex-direction: row;

    height: 50%;

    outline: 1px solid gray;
}

.img-container {
    flex-grow: 1;
    flex-basis: 0;

    justify-content: center;
    align-items: center;
    text-align: center;
}

.img-container:not(:last-child) {
    outline: 1px solid gray;
}

.img-container > img {
    object-fit: contain;
    margin: auto;
}

#user-container {
    display: flex;

    flex-direction: row;

    height: 50%;

    outline: 1px solid gray;
}

#user-container > div {
    flex-grow: 1;
    flex-basis: 0;

    outline: 1px solid gray;

    height: auto;
}

#logs-container {
    overflow-y: scroll;
    word-break: break-all;
}
</style>
