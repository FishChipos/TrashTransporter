<script setup lang="ts">
import { inject } from 'vue'

const axios: any = inject('axios');
axios.defaults.baseURL = "http://esp32.local";

function getRawOutput(): void {
    axios.get("/output/raw")
        .then((response : any) => {

        })
        .error((error : any) => {

        });
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
        <div id="settings-container">

        </div>
        <div id="control-container">

        </div>
        <div id="output-container">

        </div>
    </div>
</template>

<style scoped>
#img-output-container {
    display: flex;
    
    justify-content: space-evenly;

    max-height: 50vh;
}

.img-container > img {
    max-height: 50vh;
}

#user-container {
    display: flex;

    max-height: 50vh;
}
</style>
