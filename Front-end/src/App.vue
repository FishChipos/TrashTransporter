<script setup lang="ts">
import type { Axios, AxiosError, AxiosResponse } from 'axios';
import type { Reactive, Ref } from 'vue';
import { inject, reactive, ref } from 'vue';

type LogTimestamp = {
    hours: number;
    minutes: number;
    seconds: number;
};

type Log = {
    timestamp: LogTimestamp;
    content: string;
};

const axios: Axios = inject('axios')!;
axios.defaults.baseURL = "http://esp32.local";
axios.defaults.withCredentials = false;

let isLogsAccessible: Ref<boolean> = ref(false);

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
    .catch((error: AxiosError) => {
        isLogsAccessible.value = false;
    });
}

setInterval(readFromServer, 1000);

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
        <div id="settings-container">

        </div>
        <div id="control-container">

        </div>
        <div id="logs-container">
            <template v-if="isLogsAccessible">
                <span><b>LOGS</b></span>
                <br/>
                <!-- Lord almighty forgive me for this sin -->
                <span v-for="log in logs">
                    [{{ log.timestamp.hours <= 9 ? "0" + log.timestamp.hours : log.timestamp.hours }}:{{ log.timestamp.minutes <= 9 ? "0" + log.timestamp.minutes : log.timestamp.minutes}}:{{ log.timestamp.seconds <= 9 ? "0" + log.timestamp.seconds : log.timestamp.seconds }}] {{ log.content }}
                    <br/>
                </span>
            </template>
            <template v-else><b>ESP32 LOGS INNACCESSIBLE</b></template>
        </div>
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
    padding: 1rem;

    height: auto;
}

#logs-container {
    overflow-y: scroll;
    word-break: break-all;
}
</style>
