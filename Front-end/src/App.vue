<script setup lang="ts">
import type { Ref } from 'vue';
import { inject, ref } from 'vue';

import type { Axios, AxiosError, AxiosResponse } from 'axios';

import Container from './components/Container.vue';

import type { Log } from './components/LogItem.vue';
import LogItem from './components/LogItem.vue';

import Controller from './components/Controller.vue';

import ImageStream from './components/ImageStream.vue';

// Set up the Axios HTTP handler.
const axios: Axios = inject('axios')!;
axios.defaults.baseURL = "http://esp32.local";
axios.defaults.withCredentials = false;

// Flags.
let isLogsAccessible: Ref<boolean> = ref(false);
let isManualControl: Ref<boolean> = ref(false);

// Internal list of logs received from the webserver.
const logs: Ref<Log[]> = ref([]);

function getLogsFromServer(): void {
    axios.get("/logs", {
        timeout: 5000,
    })
    .then((response: AxiosResponse) => {
        clearInterval(readFromServerInterval);
        readFromServerInterval = setInterval(getLogsFromServer, 1000);

        isLogsAccessible.value = true;
        if (logs.value.length > response.data.count) {
            logs.value = [];
        }

        for (let log = logs.value.length; log < response.data.count; log++) {
            logs.value.push(response.data.logs[log]);
        }
    })
    .catch((_error: AxiosError) => {
        // Have longer pings when errors happen.
        clearInterval(readFromServerInterval);
        readFromServerInterval = setInterval(getLogsFromServer, 5000);

        isLogsAccessible.value = false;
    });
}

let readFromServerInterval = setInterval(getLogsFromServer, 1000);

function toggleManualControl(): void {
    isManualControl.value = !isManualControl.value;

    axios.patch("/settings/manualcontrol", {}, {
        timeout: 5000,
        params: {
            value: isManualControl.value
        }
    })
    .then((_response: AxiosResponse) => {

    })
    .catch((_error: AxiosError) => {

    });
}

</script>

<template>
    <div id="root-container">
        <div id="top-wrapper">
            <Container class="container" :no-padding="true">
                <template v-slot:title>MAP</template>
            </Container>
            <div class="vertical-separator"/>
            <Container class="container" :no-padding="true">
                <template v-slot:title>RAW IMAGE OUTPUT</template>
                <template v-slot:body>
                    <ImageStream 
                        imageUrl="http://esp32.local/output/raw"
                        imageAlt="Raw image output."
                    />
                </template>
            </Container>
        </div>
        <div class="horizontal-separator"/>
        <div id="bottom-wrapper">
            <Container class="container">
                <template v-slot:title>SETTINGS</template>
                <template v-slot:body>
                    <label class="setting">
                        <input type="checkbox" @click="toggleManualControl()"/>
                        Manual Control
                    </label>
                </template>
            </Container>
            <div class="vertical-separator"/>
            <Container class="container">
                <template v-slot:body>
                    <Controller
                        :isDisabled="!isManualControl"
                    />
                </template>
            </Container>
            <div class="vertical-separator"/>
            <Container class="container" :isScrollable="true">
                <template v-slot:title>
                    <template v-if="isLogsAccessible">
                        LOGS
                    </template>
                    <template v-else>
                        LOGS NOT ACCESSIBLE
                    </template>
                </template>
                <template v-slot:body>
                    <template v-if="isLogsAccessible">
                        <LogItem
                            v-for="log in logs"
                            :log="log"
                        />
                    </template>
                    <template v-else>
                        Common causes:
                        <ul>
                            <li>The webserver is inactive.</li>
                            <li>The webserver is not on the same network as this device.</li>
                            <li>Poor network conditions.</li>
                        </ul>
                    </template>
                </template>
            </Container>
        </div>
    </div>
</template>

<style scoped>
ul {
    margin: 0;
    padding-left: 20px;
}

.vertical-separator {
    height: 100%;
    border-right: 2px solid var(--main-color);
}

.horizontal-separator {
    width: 100%;
    border-bottom: 2px solid var(--main-color);
}

.container {
    flex-grow: 1;
    flex-basis: 0;

    height: auto;

    
}

.setting, .setting input {
    cursor: pointer;
    user-select: none;
}

.setting:hover {
    color: var(--accent-color);
}

#root-container {
    width: 100%;
    height: 100%;

    outline: 2px solid var(--main-color);
}

#top-wrapper {
    display: flex;

    flex-direction: row;

    height: 50%;
}

#bottom-wrapper {
    display: flex;

    flex-direction: row;

    height: 50%;
}

</style>
