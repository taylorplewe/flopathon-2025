<script setup>
import { computed, ref } from 'vue';
import { onKeyStroke, onStartTyping, useInterval } from '@vueuse/core';

const TYPING_TEST_WORDS = "Bacon ipsum dolor amet short loin landjaeger pork loin, beef sirloin burgdoggen hamburger pig doner. Ground round jerky tail buffalo, pastrami tongue sausage turducken pig chuck t-bone capicola shankle tri-tip. Tenderloin alcatra jerky turkey t-bone. Sausage andouille porchetta kevin drumstick beef. Strip steak swine jerky ribeye cow chicken pig meatloaf boudin ground round sausage turkey beef ribs pastrami. Biltong boudin bacon drumstick, andouille beef ribs beef shoulder. Tail shoulder turducken, chicken frankfurter tri-tip turkey beef ribs. Shankle doner brisket jowl boudin filet mignon. Boudin pork loin brisket spare ribs frankfurter rump. Tongue sausage turkey chislic pork chop chuck beef ribeye turducken. Cow doner jowl sirloin, short loin pork chop rump ground round buffalo venison turducken pork loin. Bresaola beef filet mignon shankle tri-tip hamburger corned beef shoulder chicken picanha ground round bacon jerky pork. Pig tail prosciutto kielbasa beef pork, jerky short ribs meatloaf ground round fatback ham salami. Drumstick tail ball tip, chicken pork loin tenderloin fatback jerky swine alcatra corned beef pastrami short ribs. Short ribs ribeye burgdoggen pork belly. Meatloaf frankfurter bresaola tongue andouille, turducken ball tip pancetta ham jerky picanha shankle chicken shank buffalo. Sausage jerky prosciutto meatloaf, cow chuck landjaeger short loin meatball tri-tip hamburger salami corned beef chicken tenderloin. Flank drumstick biltong pastrami, cupim filet mignon jowl pork loin beef ribs swine bacon pig picanha. Chislic pork chop jerky tri-tip porchetta landjaeger sirloin chuck corned beef pastrami picanha pancetta biltong chicken. Meatloaf frankfurter boudin short ribs strip steak ball tip."

const { counter, reset, pause, resume } = useInterval(1000, { controls: true, immediate: false });

const userTypedWords = ref('');
const typingTestStarted = ref(false);

const  wordsPerMinute = computed(() => {
    if (!userTypedWords.value) return 0;

    const wordsPerSecond = (userTypedWords.value.split(' ').length / counter.value) * 60;

    if (wordsPerSecond > 100) {
        return 100; // Cap the value at 100%
    }

    return Math.floor(wordsPerSecond);
});

onStartTyping(() => {
    if (typingTestStarted.value) return;

    typingTestStarted.value = true;
    resume();
});

onKeyStroke((e) => {
    if (!typingTestStarted.value) {
        typingTestStarted.value = true;
        resume();
    }

    if (e.key === 'Enter' || e.key === 'Shift' || e.key === 'Control' || e.key === 'Alt') {
        return;
    }

    if (e.key === 'Backspace') {
        userTypedWords.value = userTypedWords.value.slice(0, -1);
        return;
    } else {
        userTypedWords.value += e.key;
    }
});
</script>

<template>
    <div class="typing-powered-volume-slider">
        <div class="typing-test">
            <p>{{ TYPING_TEST_WORDS }}</p>
        </div>
        <p>{{ userTypedWords }}</p>
        <div class="slider-container">
            <div>
                <label>Volume: {{ wordsPerMinute }}%</label>
                <input
                    type="range"
                    min="0"
                    max="100"
                    :value="wordsPerMinute"
                    class="slider"
                    disabled
                />
            </div>
        </div>
    </div>
</template>

<style scoped>
.typing-powered-volume-slider {
    height: 100%;
    width: 100%;

    .typing-test {
        height: 20px;
        width: 100%;
        margin-top: 40px;
        white-space: nowrap;
    }

    .slider-container {
        width: 200px;
        margin: 0 auto;
        padding: 20px;
        background-color: rgba(0, 0, 0, 0.5);

        input {
            width: 100%;
            cursor: not-allowed
        }
    }
}
</style>
