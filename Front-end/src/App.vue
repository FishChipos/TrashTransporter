<script setup lang="ts">
import { inject, ref } from 'vue'

const axios: any = inject('axios');
axios.defaults.baseURL = "http://esp32.local";

const logs = ref("");

function readFromServer() {
    axios.get("/logs")
    .then((response: any) => {
        for (const log of response.data.logs) {
            logs.value += `[${log.timestamp.hours}:${log.timestamp.minutes}:${log.timestamp.seconds}] `;
            logs.value += log.content;

            logs.value += "\n";
        }
    })
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
            <pre>{{ logs }}</pre>
        </div>
    </div>
</template>

<style scoped>
/* Excuse the horrendous CSS. */
#img-output-container {
    display: flex;

    height: 50%;

    outline: 1px solid gray;
}

.img-container {
    width: 100%;
}

.img-container:not(:last-child) {
    outline: 1px solid gray;
}

.img-container > img {
    object-fit: contain;
}

#user-container {
    display: flex;

    height: 50%;

    outline: 1px solid gray;

}

#user-container > div {
    flex-grow: 1;
    flex-basis: 0;

    outline: 1px solid gray;
    padding: 1rem;
}

#logs-container {
    overflow-y: scroll;

}
</style>
